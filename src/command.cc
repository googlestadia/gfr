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

namespace GFR {

static std::atomic<uint16_t> command_buffer_marker_high_bits{1};

CommandBuffer::CommandBuffer(Device* p_device, VkCommandPool vk_command_pool,
                             VkCommandBuffer vk_command_buffer,
                             const VkCommandBufferAllocateInfo* allocate_info,
                             bool has_buffer_marker)
    : device_(p_device),
      vk_command_pool_(vk_command_pool),
      vk_command_buffer_(vk_command_buffer),
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
    } else {
      // We have top and bottom markers initialized. We need to set begin and
      // end command buffer marker values.
      begin_marker_value_ =
          ((uint32_t)command_buffer_marker_high_bits.fetch_add(1)) << 16;
      // Double check begin_marker_value to make sure it's not zero.
      if (!begin_marker_value_) {
        begin_marker_value_ =
            ((uint32_t)command_buffer_marker_high_bits.fetch_add(1)) << 16;
      }
      end_marker_value_ = begin_marker_value_ | 0x0000FFFF;
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
    WriteMarker(MarkerPosition::kTop, begin_marker_value_ + 1);
    WriteMarker(MarkerPosition::kBottom, begin_marker_value_ + 1);
  }
}

void CommandBuffer::WriteEndCommandBufferMarker() {
  if (has_buffer_marker_) {
    WriteMarker(MarkerPosition::kBottom, end_marker_value_);
  }
}

void CommandBuffer::WriteBeginCommandExecutionMarker(uint32_t command_id) {
  if (has_buffer_marker_) {
    WriteMarker(MarkerPosition::kTop, begin_marker_value_ + command_id);
  }
}

void CommandBuffer::WriteEndCommandExecutionMarker(uint32_t command_id) {
  if (has_buffer_marker_) {
    WriteMarker(MarkerPosition::kBottom, begin_marker_value_ + command_id);
  }
}

bool CommandBuffer::WasSubmittedToQueue() const {
  return buffer_state_ == CommandBufferState::kPending;
}

bool CommandBuffer::StartedExecution() const {
  return (ReadMarker(MarkerPosition::kTop) >= begin_marker_value_);
}

bool CommandBuffer::CompletedExecution() const {
  return (ReadMarker(MarkerPosition::kBottom) == end_marker_value_);
}

void CommandBuffer::Reset() {
  buffer_state_ = CommandBufferState::kInitialReset;

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
  buffer_state_ = CommandBufferState::kPending;
  submitted_queue_ = queue;
  submitted_fence_ = fence;
}

// Custom command buffer functions (not autogenerated).
VkResult CommandBuffer::PreBeginCommandBuffer(
    VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo const* pBeginInfo) {
  // Reset state on Begin.
  Reset();
  tracker_.TrackPreBeginCommandBuffer(commandBuffer, pBeginInfo);
  return VK_SUCCESS;
}

VkResult CommandBuffer::PostBeginCommandBuffer(
    VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo const* pBeginInfo,
    VkResult result) {
  // Begin recording commands.
  buffer_state_ = CommandBufferState::kRecording;

  if (pBeginInfo->flags & VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT) {
    cb_simultaneous_use_ = true;
  }

  if (cb_level_ == VK_COMMAND_BUFFER_LEVEL_SECONDARY &&
      pBeginInfo->pInheritanceInfo) {
    if (!scb_inheritance_info_) {
      scb_inheritance_info_ = GFR::GfrNew<VkCommandBufferInheritanceInfo>();
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

VkResult CommandBuffer::PreEndCommandBuffer(VkCommandBuffer commandBuffer) {
  tracker_.TrackPreEndCommandBuffer(commandBuffer);

  if (has_buffer_marker_) {
    WriteEndCommandBufferMarker();
  }
  return VK_SUCCESS;
}

VkResult CommandBuffer::PostEndCommandBuffer(VkCommandBuffer commandBuffer,
                                             VkResult result) {
  tracker_.TrackPostEndCommandBuffer(commandBuffer);
  buffer_state_ = CommandBufferState::kExecutable;

  return result;
}

VkResult CommandBuffer::PreResetCommandBuffer(VkCommandBuffer commandBuffer,
                                              VkCommandBufferResetFlags flags) {
  Reset();
  return VK_SUCCESS;
}

VkResult CommandBuffer::PostResetCommandBuffer(VkCommandBuffer commandBuffer,
                                               VkCommandBufferResetFlags flags,
                                               VkResult result) {
  return result;
}

uint32_t CommandBuffer::GetLastStartedCommand() const {
  auto marker = ReadMarker(MarkerPosition::kTop);
  return marker - begin_marker_value_;
}

uint32_t CommandBuffer::GetLastCompleteCommand() const {
  auto marker = ReadMarker(MarkerPosition::kBottom);
  if (marker == end_marker_value_) {
    return tracker_.GetCommands().back().id;
  }
  return marker - begin_marker_value_;
}

CommandBufferState CommandBuffer::GetCommandBufferState() const {
  if (!has_buffer_marker_ ||
      (IsPrimaryCommandBuffer() && !WasSubmittedToQueue())) {
    return buffer_state_;
  }
  // If the command buffer is submitted and markers can be used, determine the
  // execution state of the command buffer.
  if (!StartedExecution()) {
    return CommandBufferState::kSubmittedExecutionNotStarted;
  }
  if (!CompletedExecution()) {
    return CommandBufferState::kSubmittedExecutionIncomplete;
  }
  return CommandBufferState::kSubmittedExecutionCompleted;
}

CommandBufferState CommandBuffer::GetSecondaryCommandBufferState(
    CommandState vkcmd_execute_commands_command_state) const {
  assert(vkcmd_execute_commands_command_state != CommandState::kInvalidState);
  if (vkcmd_execute_commands_command_state ==
      CommandState::kCommandNotSubmitted) {
    return CommandBufferState::kNotSubmitted;
  }
  if (vkcmd_execute_commands_command_state ==
      CommandState::kCommandNotStarted) {
    return CommandBufferState::kSubmittedExecutionNotStarted;
  }
  if (vkcmd_execute_commands_command_state == CommandState::kCommandCompleted) {
    return CommandBufferState::kSubmittedExecutionCompleted;
  }
  return GetCommandBufferState();
}

std::string CommandBuffer::PrintCommandBufferState(
    CommandBufferState cb_state) const {
  switch (cb_state) {
    case CommandBufferState::kInitial:
      return "CREATED";
    case CommandBufferState::kRecording:
      return "BEGIN_CALLED";
    case CommandBufferState::kExecutable:
      return "END_CALLED";
    case CommandBufferState::kPending:
      return "SUBMITTED_NO_MORE_INFO";
    case CommandBufferState::kInvalid:
      return "INVALID";
    case CommandBufferState::kInitialReset:
      return "RESET";
    case CommandBufferState::kSubmittedExecutionNotStarted:
      return "SUBMITTED_EXECUTION_NOT_STARTED";
    case CommandBufferState::kSubmittedExecutionIncomplete:
      return "SUBMITTED_EXECUTION_INCOMPLETE";
    case CommandBufferState::kSubmittedExecutionCompleted:
      return "SUBMITTED_EXECUTION_COMPLETE";
    case CommandBufferState::kNotSubmitted:
      return "NOT_SUBMITTED";
    default:
      assert(true);
      return "UNKNOWN";
  }
}

CommandState CommandBuffer::GetCommandState(CommandBufferState cb_state,
                                            const Command& command) const {
  if (IsPrimaryCommandBuffer() && !WasSubmittedToQueue()) {
    return CommandState::kCommandNotSubmitted;
  }
  if (!has_buffer_marker_) {
    return CommandState::kCommandPending;
  }
  if (!IsPrimaryCommandBuffer()) {
    if (cb_state == CommandBufferState::kNotSubmitted) {
      return CommandState::kCommandNotSubmitted;
    }
    if (cb_state == CommandBufferState::kSubmittedExecutionNotStarted) {
      return CommandState::kCommandNotStarted;
    }
    if (cb_state == CommandBufferState::kSubmittedExecutionCompleted) {
      return CommandState::kCommandCompleted;
    }
    assert(cb_state == CommandBufferState::kSubmittedExecutionIncomplete);
  }
  if (command.id > GetLastStartedCommand()) {
    return CommandState::kCommandNotStarted;
  }
  if (command.id <= GetLastCompleteCommand()) {
    return CommandState::kCommandCompleted;
  }
  return CommandState::kCommandIncomplete;
}

std::string CommandBuffer::PrintCommandState(CommandState cm_state) const {
  switch (cm_state) {
    case CommandState::kCommandNotSubmitted:
      return "NOT_SUBMITTED";
    case CommandState::kCommandPending:
      return "SUBMITTED_NO_MORE_INFO";
    case CommandState::kCommandNotStarted:
      return "SUBMITTED_EXECUTION_NOT_STARTED";
    case CommandState::kCommandIncomplete:
      return "SUBMITTED_EXECUTION_INCOMPLETE";
    case CommandState::kCommandCompleted:
      return "SUBMITTED_EXECUTION_COMPLETE";
    default:
      assert(true);
      return "UNKNOWN";
  }
}

bool CommandBuffer::DumpCommand(const Command& command, std::ostream& os,
                                const std::string& indent) {
  size_t stream_pos_0 = os.tellp();

  tracker_.SetNameResolver(&device_->GetObjectInfoDB());
  tracker_.PrintCommandParameters(os, command,
                                  static_cast<uint32_t>(indent.length() - 1));

  size_t stream_pos_1 = os.tellp();
  bool wrote_output = (stream_pos_0 != stream_pos_1);
  return wrote_output;
}

bool CommandBuffer::DumpCmdExecuteCommands(const Command& command,
                                           CommandState command_state,
                                           std::ostream& os,
                                           CommandBufferDumpOptions options,
                                           const std::string& indent) {
  size_t stream_pos_0 = os.tellp();
  auto args = reinterpret_cast<CmdExecuteCommandsArgs*>(command.parameters);
  auto pindent1 = GFR::IncreaseIndent(indent);
  auto pindent2 = GFR::IncreaseIndent(pindent1);
  auto pindent3 = GFR::IncreaseIndent(pindent2);
  auto pindent4 = GFR::IncreaseIndent(pindent3);
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
      auto secondary_command_buffer =
          GFR::GetGfrCommandBuffer(args->pCommandBuffers[i]);
      if (secondary_command_buffer) {
        secondary_command_buffer->DumpContents(os, options, pindent3,
                                               submit_info_id_, command_state);
      }
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
    case Command::Type::kCmdDraw:
    case Command::Type::kCmdDrawIndexed:
    case Command::Type::kCmdDrawIndirect:
    case Command::Type::kCmdDrawIndexedIndirect:
      return VK_PIPELINE_BIND_POINT_GRAPHICS;

    case Command::Type::kCmdDispatch:
    case Command::Type::kCmdDispatchIndirect:
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
  if (cmd.type == Command::Type::kCmdBindDescriptorSets) {
    if (cmd.parameters) {
      // Update the active descriptorsets for this bind point.
      auto args = reinterpret_cast<CmdBindDescriptorSetsArgs*>(cmd.parameters);
      bound_descriptors_[args->pipelineBindPoint].Bind(
          args->firstSet, args->descriptorSetCount, args->pDescriptorSets);
    }
  } else if (cmd.type == Command::Type::kCmdBindPipeline) {
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
    case Command::Type::kCmdDraw:
    case Command::Type::kCmdDrawIndexed:
    case Command::Type::kCmdDrawIndirect:
    case Command::Type::kCmdDrawIndexedIndirect:
      bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS;
      break;

    case Command::Type::kCmdDispatch:
    case Command::Type::kCmdDispatchIndirect:
      bind_point = VK_PIPELINE_BIND_POINT_COMPUTE;
      break;

    default:
      break;
  }

  if (-1 != bind_point) {
    os << indent << "internalState:";
    auto indent2 = GFR::IncreaseIndent(indent);
    os << indent2 << "pipeline:";
    bound_pipelines_[bind_point]->Print(os, name_resolver, indent2);
    os << indent2 << "descriptorSets:";
    bound_descriptors_[bind_point].Print(device_, os, indent2);
    return true;
  }

  return false;
}

void CommandBuffer::DumpContents(
    std::ostream& os, CommandBufferDumpOptions options,
    const std::string& indent, uint64_t secondary_cb_submit_info_id,
    CommandState vkcmd_execute_commands_command_state) {
  auto num_commands = tracker_.GetCommands().size();
  StringArray indents = {indent};
  for (uint32_t i = 1; i < 4; i++) {
    indents.push_back(GFR::IncreaseIndent(indents[i - 1]));
  }
  os << indents[0] << "- # CommandBuffer:"
     << device_->GetObjectInfo((uint64_t)vk_command_buffer_, indents[1])
     << indents[1]
     << "device:" << device_->GetObjectInfo((uint64_t)device_, indents[2]);
  if (has_buffer_marker_) {
    os << indents[1]
       << "beginMarkerValue: " << GFR::Uint32ToStr(begin_marker_value_)
       << indents[1]
       << "endMarkerValue: " << GFR::Uint32ToStr(end_marker_value_);
    os << indents[1] << "topMarkerBuffer: "
       << GFR::Uint32ToStr(ReadMarker(MarkerPosition::kTop)) << indents[1]
       << "bottomMarkerBuffer: "
       << GFR::Uint32ToStr(ReadMarker(MarkerPosition::kBottom));
  }
  os << indents[1] << "submitInfoId: ";
  if (IsPrimaryCommandBuffer()) {
    os << submit_info_id_;
  } else {
    os << secondary_cb_submit_info_id;
  }
  os << indents[1] << "commandPool:"
     << device_->GetObjectInfo((uint64_t)vk_command_pool_, indents[2])
     << indents[1] << "level: ";
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

  bool dump_commands = (options & CommandBufferDumpOption::kDumpAllCommands);

  os << indents[1] << "status: ";
  CommandBufferState cb_state;
  if (IsPrimaryCommandBuffer()) {
    cb_state = GetCommandBufferState();
  } else {
    cb_state =
        GetSecondaryCommandBufferState(vkcmd_execute_commands_command_state);
  }
  os << PrintCommandBufferState(cb_state);
  if (cb_state == CommandBufferState::kSubmittedExecutionIncomplete) {
    os << indents[1] << "lastStartedCommand: " << GetLastStartedCommand()
       << indents[1] << "lastCompletedCommand: " << GetLastCompleteCommand();
    dump_commands = true;
  }
  if (buffer_state_ == CommandBufferState::kPending) {
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
      auto command_name = Command::GetCommandName(command);
      auto command_state = GetCommandState(cb_state, command);
      os << indents[2] << "- # Command:" << indents[3] << "id: " << command.id
         << "/" << num_commands << indents[3] << "markerValue: "
         << GFR::Uint32ToStr(begin_marker_value_ + command.id) << indents[3]
         << "name: " << command_name << indents[3] << "state: ["
         << PrintCommandState(command_state) << "]" << indents[3]
         << "parameters:" << std::endl;

      state.Mutate(command);
      // For vkCmdExecuteCommands, GFR prints all the information about the
      // recorded command buffers. For every other command, GFR prints the
      // arguments without going deep into printing objects themselves.
      if (strcmp(command_name, "vkCmdExecuteCommands") != 0) {
        DumpCommand(command, os, indents[3]);
      } else {
        DumpCmdExecuteCommands(command, command_state, os, options, indents[3]);
      }
      state.Print(command, indents[3], os, device_->GetObjectInfoDB());
      if (command_state == CommandState::kCommandIncomplete) {
        HandleIncompleteCommand(command, state);
      }

      // To make this message more visible, we put it in a special
      // Command entry.
      if (cb_state == CommandBufferState::kSubmittedExecutionIncomplete &&
          command.id == GetLastCompleteCommand()) {
        os << indents[2] << "- # Command:" << indents[3] << "id: " << command.id
           << "/" << num_commands << indents[3] << "message: "
           << "'>>>>>>>>>>>>>> LAST COMPLETE COMMAND <<<<<<<<<<<<<<'";
      }
    }
  }
}

// =============================================================================
// Include the generated command tracking code
// =============================================================================
#include "command.cc.inc"

}  // namespace GFR
