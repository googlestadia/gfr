/*
 Copyright 2018 Google Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "command.h"

#include <cassert>
#include <cstring>
#include <memory>
#include <sstream>

#include "device.h"
#include "gfr.h"
#include "util.h"

namespace gfr {

static constexpr uint32_t kCommandBeginMarker = 0xBBBBBBBB;
static constexpr uint32_t kCommandEndMarker = 0xEEEEEEEE;

CommandBuffer::CommandBuffer(Device* p_device, VkCommandPool vk_command_pool,
                             VkCommandBuffer vk_command_buffer,
                             VkCommandBuffer wrapped_command_buffer,
                             const VkCommandBufferAllocateInfo* allocate_info,
                             bool has_buffer_marker)
    : device_(p_device),
      vk_command_pool_(vk_command_pool),
      vk_command_buffer_(vk_command_buffer),
      wrapped_command_buffer_(wrapped_command_buffer),
      cb_level_(allocate_info->level),
      has_buffer_marker_(has_buffer_marker) {
  if (has_buffer_marker_) {
    top_marker_.type = MarkerType::kUint32;
    bottom_marker_.type = MarkerType::kUint32;
    bool top_marker_is_valid = p_device->AllocateMarker(&top_marker_);
    if (!top_marker_is_valid || !p_device->AllocateMarker(&bottom_marker_)) {
      std::cerr << "GFR warning: Cannot acquire markers. Not tracking "
                   "VkCommandBuffer "
                << device_->GetObjectName((uint64_t)vk_command_buffer)
                << std::endl;
      has_buffer_marker_ = false;
      if (top_marker_is_valid) {
        p_device->FreeMarker(top_marker_);
      }
    }
  }
}

CommandBuffer::~CommandBuffer() {
  if (scb_inheritance_info_) {
    delete scb_inheritance_info_;
  }
  if (has_buffer_marker_) {
    device_->FreeMarker(top_marker_);
    device_->FreeMarker(bottom_marker_);
  }
}

void CommandBuffer::SetSubmitInfoId(uint64_t submit_info_id) {
  submit_info_id_ = submit_info_id;
}

void CommandBuffer::WriteMarker(MarkerPosition position,
                                uint32_t marker_value) {
  assert(has_buffer_marker_);
  auto& marker =
      (position == MarkerPosition::kTop) ? top_marker_ : bottom_marker_;
  auto pipelineStage = (position == MarkerPosition::kTop)
                           ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
                           : VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  device_->CmdWriteBufferMarkerAMD(vk_command_buffer_, pipelineStage,
                                   marker.buffer, marker.offset, marker_value);
}

uint32_t CommandBuffer::ReadMarker(MarkerPosition position) const {
  assert(has_buffer_marker_);
  auto& marker =
      (position == MarkerPosition::kTop) ? top_marker_ : bottom_marker_;
  auto value = *(uint32_t*)(marker.cpu_mapped_address);
  return value;
}

void CommandBuffer::WriteBeginCommandBufferMarker() {
  if (has_buffer_marker_) {
    // GFR log lables the commands inside a command buffer as follows:
    // - vkBeginCommandBuffer: 1
    // - n vkCmd commands recorded into command buffer: 2 ... n+1
    // - vkEndCommandBuffer: n+2
    WriteMarker(MarkerPosition::kTop, kCommandBeginMarker + 1);
    WriteMarker(MarkerPosition::kBottom, kCommandBeginMarker + 1);
  }
}

void CommandBuffer::WriteEndCommandBufferMarker() {
  if (has_buffer_marker_) {
    WriteMarker(MarkerPosition::kBottom, kCommandEndMarker);
  }
}

void CommandBuffer::WriteBeginCommandExecutionMarker(uint32_t command_id) {
  if (has_buffer_marker_) {
    WriteMarker(MarkerPosition::kTop, kCommandBeginMarker + command_id);
  }
}

void CommandBuffer::WriteEndCommandExecutionMarker(uint32_t command_id) {
  if (has_buffer_marker_) {
    WriteMarker(MarkerPosition::kBottom, kCommandBeginMarker + command_id);
  }
}

bool CommandBuffer::WasSubmittedToQueue() const {
  return buffer_state_ == BufferState::kPending;
}

bool CommandBuffer::StartedExecution() const {
  return (ReadMarker(MarkerPosition::kTop) >= kCommandBeginMarker);
}

bool CommandBuffer::CompletedExecution() const {
  return (ReadMarker(MarkerPosition::kBottom) == kCommandEndMarker);
}

void CommandBuffer::Reset() {
  buffer_state_ = BufferState::kInitial;

  // Reset marker state.
  if (has_buffer_marker_) {
    *(uint32_t*)(top_marker_.cpu_mapped_address) = 0;
    *(uint32_t*)(bottom_marker_.cpu_mapped_address) = 0;
  }

  // Clear inheritance info
  if (scb_inheritance_info_) {
    delete scb_inheritance_info_;
    scb_inheritance_info_ = nullptr;
  }

  // Clear commands and internal state.
  tracker_.Reset();
  submitted_queue_ = VK_NULL_HANDLE;
  submitted_fence_ = VK_NULL_HANDLE;
}

void CommandBuffer::QueueSubmit(VkQueue queue, VkFence fence) {
  buffer_state_ = BufferState::kPending;
  submitted_queue_ = queue;
  submitted_fence_ = fence;
}

// Custom command buffer functions (not autogenerated).
VkResult CommandBuffer::PreBeginCommandBuffer(
    VkCommandBuffer wrappedCommandBuffer, VkCommandBuffer commandBuffer,
    VkCommandBufferBeginInfo const* pBeginInfo) {
  // Reset state on Begin.
  Reset();
  tracker_.TrackPreBeginCommandBuffer(commandBuffer, pBeginInfo);
  return VK_SUCCESS;
}

VkResult CommandBuffer::PostBeginCommandBuffer(
    VkCommandBuffer wrappedCommandBuffer, VkCommandBuffer commandBuffer,
    VkCommandBufferBeginInfo const* pBeginInfo, VkResult result) {
  // Begin recording commands.
  buffer_state_ = BufferState::kRecording;

  if (pBeginInfo->flags & VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT) {
    cb_simultaneous_use_ = true;
  }

  if (cb_level_ == VK_COMMAND_BUFFER_LEVEL_SECONDARY &&
      pBeginInfo->pInheritanceInfo) {
    if (!scb_inheritance_info_) {
      scb_inheritance_info_ = gfr::GfrNew<VkCommandBufferInheritanceInfo>();
    }
    *scb_inheritance_info_ = *pBeginInfo->pInheritanceInfo;
  }

  // All our markers go in post for begin because they must be recorded after
  // the driver starts recording
  if (has_buffer_marker_) {
    WriteBeginCommandBufferMarker();
  }

  tracker_.TrackPostBeginCommandBuffer(commandBuffer, pBeginInfo);
  return result;
}

VkResult CommandBuffer::PreEndCommandBuffer(
    VkCommandBuffer wrappedCommandBuffer, VkCommandBuffer commandBuffer) {
  tracker_.TrackPreEndCommandBuffer(wrappedCommandBuffer);

  if (has_buffer_marker_) {
    WriteEndCommandBufferMarker();
  }
  return VK_SUCCESS;
}

VkResult CommandBuffer::PostEndCommandBuffer(
    VkCommandBuffer wrappedCommandBuffer, VkCommandBuffer commandBuffer,
    VkResult result) {
  tracker_.TrackPostEndCommandBuffer(wrappedCommandBuffer);
  buffer_state_ = BufferState::kExecutable;

  return result;
}

VkResult CommandBuffer::PreResetCommandBuffer(
    VkCommandBuffer wrappedCommandBuffer, VkCommandBuffer commandBuffer,
    VkCommandBufferResetFlags flags) {
  Reset();
  return VK_SUCCESS;
}

VkResult CommandBuffer::PostResetCommandBuffer(
    VkCommandBuffer wrappedCommandBuffer, VkCommandBuffer commandBuffer,
    VkCommandBufferResetFlags flags, VkResult result) {
  return result;
}

uint32_t CommandBuffer::GetLastStartedCommand() const {
  auto marker = ReadMarker(MarkerPosition::kTop);
  return marker - kCommandBeginMarker;
}

uint32_t CommandBuffer::GetLastCompleteCommand() const {
  auto marker = ReadMarker(MarkerPosition::kBottom);
  if (marker == kCommandEndMarker) {
    return tracker_.GetCommands().back().id;
  }
  return marker - kCommandBeginMarker;
}

CommandBuffer::CommandStatus::Status CommandBuffer::GetCommandStatus(
    const Command& command) const {
  if (!has_buffer_marker_) {
    return CommandStatus::kUnknown;
  }
  uint32_t last_started_command_id = GetLastStartedCommand();
  uint32_t last_complete_command_id = GetLastCompleteCommand();
  if (last_started_command_id < command.id) {
    return CommandStatus::kPending;
  }
  if (last_complete_command_id >= command.id) {
    return CommandStatus::kCompleted;
  }
  return CommandStatus::kIncomplete;
}

std::string CommandBuffer::PrintCommandStatus(
    CommandStatus::Status status) const {
  switch (status) {
    case CommandStatus::kPending:
      return "[PENDING]";
      break;
    case CommandStatus::kCompleted:
      return "[COMPLETED]";
      break;
    case CommandStatus::kIncomplete:
      return "[INCOMPLETE]";
      break;
    default:
    case CommandStatus::kUnknown:
      return "[UNKNOWN]";
      break;
  }
}

bool CommandBuffer::DumpCommand(const Command& command, std::ostream& os,
                                const std::string& indent) {
  size_t stream_pos_0 = os.tellp();

  tracker_.SetNameResolver(os, &device_->GetObjectInfoDB());
  tracker_.PrintCommandParameters(os, command, indent);

  size_t stream_pos_1 = os.tellp();
  bool wrote_output = (stream_pos_0 != stream_pos_1);
  return wrote_output;
}

bool CommandBuffer::DumpCmdExecuteCommands(const Command& command,
                                           std::ostream& os,
                                           CommandBufferDumpOptions options,
                                           const std::string& indent) {
  size_t stream_pos_0 = os.tellp();
  auto args = reinterpret_cast<CmdExecuteCommandsArgs*>(command.parameters);
  auto pindent1 = gfr::IncreaseIndent(indent);
  auto pindent2 = gfr::IncreaseIndent(pindent1);
  auto pindent3 = gfr::IncreaseIndent(pindent2);
  auto pindent4 = gfr::IncreaseIndent(pindent3);
  os << pindent1 << "- # parameter:";
  os << pindent2 << "name: commandBuffer";
  os << pindent2 << "value: " << args->commandBuffer;
  os << pindent1 << "- # parameter:";
  os << pindent2 << "name: commandBufferCount";
  os << pindent2 << "value: " << args->commandBufferCount;
  os << pindent1 << "- # parameter:";
  os << pindent2 << "name: pCommandBuffers";
  if (args->pCommandBuffers && args->commandBufferCount > 0) {
    os << pindent2 << "commandBuffers:";
    for (uint32_t i = 0; i < args->commandBufferCount; i++) {
      device_->DumpSecondaryCommandBuffer(
          args->pCommandBuffers[i], submit_info_id_, os, options, pindent3);
    }
  } else {
    os << pindent2 << "value: nullptr";
  }
  size_t stream_pos_1 = os.tellp();
  bool wrote_output = (stream_pos_0 != stream_pos_1);
  return wrote_output;
}

// Mutable internal command buffer state used to determine what the
// state should be at a given command
class CommandBufferInternalState {
 public:
  CommandBufferInternalState(Device* device) : device_(device) {}

  // Mutate the internal state by the command.
  void Mutate(const Command& cmd);

  // Print the relevant state for the command.
  bool Print(const Command& cmd, const std::string& indent, std::ostream& os,
             const ObjectInfoDB& name_resolver);

  const Pipeline* GetPipeline(VkPipelineBindPoint bind_point) const {
    return bound_pipelines_[bind_point];
  }

 private:
  static constexpr int kNumBindPoints = 2;  // graphics, compute

  Device* device_;
  std::array<const Pipeline*, kNumBindPoints> bound_pipelines_;
  std::array<ActiveDescriptorSets, kNumBindPoints> bound_descriptors_;
};

// Returns the pipeline used by this command or -1 if no pipeline used.
int GetCommandPipelineType(const Command& command) {
  switch (command.type) {
    case Command::Type::kDraw:
    case Command::Type::kDrawIndexed:
    case Command::Type::kDrawIndirect:
    case Command::Type::kDrawIndexedIndirect:
      return VK_PIPELINE_BIND_POINT_GRAPHICS;

    case Command::Type::kDispatch:
    case Command::Type::kDispatchIndirect:
      return VK_PIPELINE_BIND_POINT_COMPUTE;

    default:
      return -1;
  }
}

// Perform operations when a command is not completed.
// Currently used to dump shader SPIRV when a command is incomplete.
void CommandBuffer::HandleIncompleteCommand(
    const Command& command, const CommandBufferInternalState& state) const {
  // Should we write our shaders on crash?
  auto gfr_context = device_->GetGFR();
  if (!gfr_context->DumpShadersOnCrash()) {
    return;
  }

  // We only handle commands with pipelines.
  auto pipeline_type = GetCommandPipelineType(command);
  if (-1 == pipeline_type) {
    return;
  }

  auto pipeline =
      state.GetPipeline(static_cast<VkPipelineBindPoint>(pipeline_type));
  auto vk_pipeline = pipeline->GetVkPipeline();

  device_->DumpShaderFromPipeline(vk_pipeline);
}

void CommandBufferInternalState::Mutate(const Command& cmd) {
  if (cmd.type == Command::Type::kBindDescriptorSets) {
    if (cmd.parameters) {
      // Update the active descriptorsets for this bind point.
      auto args = reinterpret_cast<CmdBindDescriptorSetsArgs*>(cmd.parameters);
      bound_descriptors_[args->pipelineBindPoint].Bind(
          args->firstSet, args->descriptorSetCount, args->pDescriptorSets);
    }
  } else if (cmd.type == Command::Type::kBindPipeline) {
    if (cmd.parameters) {
      // Update the currently bound pipeline.
      auto args = reinterpret_cast<CmdBindPipelineArgs*>(cmd.parameters);
      bound_pipelines_[args->pipelineBindPoint] =
          device_->FindPipeline(args->pipeline);
    }
  }
}

bool CommandBufferInternalState::Print(const Command& cmd,
                                       const std::string& indent,
                                       std::ostream& os,
                                       const ObjectInfoDB& name_resolver) {
  int bind_point = -1;
  switch (cmd.type) {
    case Command::Type::kDraw:
    case Command::Type::kDrawIndexed:
    case Command::Type::kDrawIndirect:
    case Command::Type::kDrawIndexedIndirect:
      bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS;
      break;

    case Command::Type::kDispatch:
    case Command::Type::kDispatchIndirect:
      bind_point = VK_PIPELINE_BIND_POINT_COMPUTE;
      break;

    default:
      break;
  }

  if (-1 != bind_point) {
    os << indent << "internalState:";
    auto indent2 = gfr::IncreaseIndent(indent);
    os << indent2 << "pipeline:";
    bound_pipelines_[bind_point]->Print(os, name_resolver, indent2);
    os << indent2 << "descriptorSets:";
    bound_descriptors_[bind_point].Print(device_, os, indent2);
    return true;
  }

  return false;
}

void CommandBuffer::DumpContents(std::ostream& os,
                                 CommandBufferDumpOptions options,
                                 const std::string& indent,
                                 uint64_t secondary_cb_submit_info_id) {
  auto num_commands = tracker_.GetCommands().size();
  StringArray indents = {indent};
  for (uint32_t i = 1; i < 4; i++) {
    indents.push_back(gfr::IncreaseIndent(indents[i - 1]));
  }
  os << indents[0] << "- # CommandBuffer:"
     << device_->GetObjectInfo((uint64_t)wrapped_command_buffer_, indents[1])
     << indents[1] << "device:"
     << device_->GetObjectInfo((uint64_t)vk_command_buffer_, indents[2])
     << indents[1] << "submitInfoId: ";
  if (IsPrimaryCommandBuffer()) {
    os << submit_info_id_;
  } else {
    os << secondary_cb_submit_info_id;
  }
  os << indents[1] << "commandPool:"
     << device_->GetObjectInfo((uint64_t)vk_command_pool_, indents[2])
     << indents[1] << "Level: ";
  if (IsPrimaryCommandBuffer()) {
    os << "Primary";
  } else {
    os << "Secondary" << indents[1] << "stateInheritance: ";
    if (scb_inheritance_info_ != nullptr) {
      os << "True";
    } else {
      os << "False";
    }
  }
  os << indents[1] << "simultaneousUse: ";
  if (cb_simultaneous_use_) {
    os << "True";
  } else {
    os << "False";
  }

  size_t last_complete_command = 0;
  bool started = false;
  bool completed = false;
  bool dump_commands = (options & CommandBufferDumpOption::kDumpAllCommands);

  os << indents[1] << "status: ";
  if (!has_buffer_marker_) {
    os << "UNKNOWN";
  } else {
    started = StartedExecution();
    completed = CompletedExecution();

    if (IsPrimaryCommandBuffer() && buffer_state_ != BufferState::kPending) {
      os << "NOT_SUBMITTED";
    } else if (!started) {
      os << "EXECUTION_NOT_STARTED";
    } else if (completed) {
      os << "EXECUTION_COMPLETED";
    } else {
      last_complete_command = GetLastCompleteCommand();
      os << "EXECUTION_INCOMPLETE" << indents[1]
         << "lastStartedCommand: " << GetLastStartedCommand() << indents[1]
         << "lastCompletedCommand: " << last_complete_command;
    }
    dump_commands = dump_commands || (started && !completed);
  }
  if (buffer_state_ == BufferState::kPending) {
    os << indents[1] << "queue:"
       << device_->GetObjectInfo((uint64_t)submitted_queue_, indents[2])
       << indents[1] << "fence:"
       << device_->GetObjectInfo((uint64_t)submitted_fence_, indents[2]);
  }

  // Internal command buffer state that needs to be tracked.
  CommandBufferInternalState state(device_);

  if (dump_commands) {
    os << indents[1] << "Commands:";
    for (const auto& command : tracker_.GetCommands()) {
      auto command_status = GetCommandStatus(command);
      auto command_name = Command::GetCommandName(command);
      os << indents[2] << "- # Command:" << indents[3] << "id: " << command.id
         << "/" << num_commands << indents[3] << "name: " << command_name
         << indents[3] << "state: " << PrintCommandStatus(command_status)
         << indents[3] << "parameters:";

      state.Mutate(command);
      // For vkCmdExecuteCommands, GFR prints all the information about the
      // recorded command buffers. For every other command, GFR prints the
      // arguments without going deep into printing objects themselves.
      if (strcmp(command_name, "vkCmdExecuteCommands") != 0) {
        DumpCommand(command, os, indents[3]);
      } else {
        DumpCmdExecuteCommands(command, os, options, indents[3]);
      }
      state.Print(command, indents[3], os, device_->GetObjectInfoDB());
      if (command_status == CommandStatus::kIncomplete) {
        HandleIncompleteCommand(command, state);
      }

      // To make this message more visible, we put it in a special
      // Command entry.
      if (has_buffer_marker_ && !completed &&
          command.id == last_complete_command) {
        os << indents[2] << "- # Command:" << indents[3] << "id: " << command.id
           << "/" << num_commands << indents[3] << "message: "
           << "'>>>>>>>>>>>>>> LAST COMPLETE COMMAND <<<<<<<<<<<<<<'";
      }
    }
    // Buffer
    os << indents[1] << "MarkerBuffers:";
    if (has_buffer_marker_) {
      os << indents[2] << "status: Available" << indents[2]
         << "top_marker_buffer: "
         << gfr::Uint64ToStr(ReadMarker(MarkerPosition::kTop)) << indents[2]
         << "bottom_marker_buffer: "
         << gfr::Uint64ToStr(ReadMarker(MarkerPosition::kBottom));
    } else {
      os << indents[2]
         << "status: N/A (VK_AMD_buffer_marker extension not supported)";
    }
  }
}

// =============================================================================
// Include the generated command tracking code
// =============================================================================
#include "command.cc.inc"

}  // namespace gfr
