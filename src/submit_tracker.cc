/*
 Copyright 2020 Google Inc.

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

#include "submit_tracker.h"

#include <cstring>
#include <iomanip>
#include <sstream>

#include "device.h"
#include "gfr.h"
#include "util.h"

namespace gfr {

SubmitTracker::SubmitTracker(Device* p_device) : device_(p_device) {
}

SubmitTracker::SubmitInfo::SubmitInfo() {
  top_marker.type = MarkerType::kUint32;
  bottom_marker.type = MarkerType::kUint32;
}

SubmitInfoId SubmitTracker::RegisterSubmitInfo(
    QueueSubmitId queue_submit_index, const VkSubmitInfo* vk_submit_info) {
  // Store the handles of command buffers and semaphores
  SubmitInfo submit_info;
  // Reserve the markers
  bool top_marker_is_valid = device_->AllocateMarker(&submit_info.top_marker);
  if (!top_marker_is_valid ||
      !device_->AllocateMarker(&submit_info.bottom_marker)) {
    std::cerr << "GFR warning: Cannot acquire marker. Not tracking submit info "
              << device_->GetObjectName((uint64_t)vk_submit_info) << std::endl;
    if (top_marker_is_valid) {
      device_->FreeMarker(submit_info.top_marker);
    }
    return kInvalidSubmitInfoId;
  }
  submit_info.submit_info_id = ++submit_info_counter;
  submit_info.queue_submit_index = queue_submit_index;
  submit_info.vk_submit_info = vk_submit_info;
  for (uint32_t i = 0; i < vk_submit_info->waitSemaphoreCount; i++) {
    submit_info.wait_semaphores.push_back(vk_submit_info->pWaitSemaphores[i]);
    submit_info.wait_semaphore_values.push_back(1);
    submit_info.wait_semaphore_pipeline_stages.push_back(
        vk_submit_info->pWaitDstStageMask[i]);
  }
  for (uint32_t i = 0; i < vk_submit_info->commandBufferCount; i++) {
    submit_info.command_buffers.push_back(vk_submit_info->pCommandBuffers[i]);
  }
  for (uint32_t i = 0; i < vk_submit_info->signalSemaphoreCount; i++) {
    submit_info.signal_semaphores.push_back(
        vk_submit_info->pSignalSemaphores[i]);
    submit_info.signal_semaphore_values.push_back(1);
  }

  // Store type and initial value of the timeline semaphores.
  const VkTimelineSemaphoreSubmitInfoKHR* timeline_semaphore_info =
      FindOnChain<VkTimelineSemaphoreSubmitInfoKHR,
                  VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR>(
          vk_submit_info->pNext);
  if (timeline_semaphore_info) {
    auto semaphore_tracker = device_->GetSemaphoreTracker();
    for (uint32_t i = 0; i < timeline_semaphore_info->waitSemaphoreValueCount;
         i++) {
      if (semaphore_tracker->GetSemaphoreType(
              vk_submit_info->pWaitSemaphores[i]) ==
          VK_SEMAPHORE_TYPE_TIMELINE_KHR) {
        submit_info.wait_semaphore_values[i] =
            timeline_semaphore_info->pWaitSemaphoreValues[i];
      }
    }
    for (uint32_t i = 0; i < timeline_semaphore_info->signalSemaphoreValueCount;
         i++) {
      if (semaphore_tracker->GetSemaphoreType(
              vk_submit_info->pSignalSemaphores[i]) ==
          VK_SEMAPHORE_TYPE_TIMELINE_KHR) {
        submit_info.signal_semaphore_values[i] =
            timeline_semaphore_info->pSignalSemaphoreValues[i];
      }
    }
  }

  {
    std::lock_guard<std::mutex> lock(submit_infos_mutex_);
    submit_infos_[submit_info.submit_info_id] = submit_info;
  }
  {
    std::lock_guard<std::mutex> lock(queue_submits_mutex_);
    queue_submits_[queue_submit_index].push_back(submit_info.submit_info_id);
  }
  return submit_info.submit_info_id;
}

void SubmitTracker::StoreSubmitHelperCommandBuffersInfo(
    SubmitInfoId submit_info_id, VkCommandPool vk_pool,
    VkCommandBuffer start_marker_cb, VkCommandBuffer end_marker_cb) {
  assert(submit_info_id != kInvalidSubmitInfoId);
  std::lock_guard<std::mutex> lock(submit_infos_mutex_);
  if (submit_infos_.find(submit_info_id) != submit_infos_.end()) {
    SubmitInfo& submit_info = submit_infos_[submit_info_id];
    submit_info.helper_cbs_command_pool = vk_pool;
    submit_info.start_marker_cb = start_marker_cb;
    submit_info.end_marker_cb = end_marker_cb;
  }
}

void SubmitTracker::RecordSubmitStart(QueueSubmitId qsubmit_id,
                                      SubmitInfoId submit_info_id,
                                      VkCommandBuffer vk_command_buffer) {
  assert(submit_info_id != kInvalidSubmitInfoId);
  std::lock_guard<std::mutex> slock(submit_infos_mutex_);
  if (submit_infos_.find(submit_info_id) != submit_infos_.end()) {
    SubmitInfo& submit_info = submit_infos_[submit_info_id];
    // Write the state of the submit
    *(uint32_t*)(submit_info.top_marker.cpu_mapped_address) =
        SubmitState::kQueued;
    device_->CmdWriteBufferMarkerAMD(
        vk_command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        submit_info.top_marker.buffer, submit_info.top_marker.offset,
        SubmitState::kRunning);
    // Reset binary wait semaphores
    auto semaphore_tracker = device_->GetSemaphoreTracker();
    for (size_t i = 0; i < submit_info.wait_semaphores.size(); i++) {
      if (semaphore_tracker->GetSemaphoreType(submit_info.wait_semaphores[i]) ==
          VK_SEMAPHORE_TYPE_BINARY_KHR) {
        semaphore_tracker->WriteMarker(
            submit_info.wait_semaphores[i], vk_command_buffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0,
            {SemaphoreModifierType::kModifierQueueSubmit, qsubmit_id});
      }
    }
  } else {
    std::cerr << "GFR Warning: No previous record of queued submit in submit "
                 "tracker: "
              << submit_info_id << std::endl;
  }
}

void SubmitTracker::RecordSubmitFinish(QueueSubmitId qsubmit_id,
                                       SubmitInfoId submit_info_id,
                                       VkCommandBuffer vk_command_buffer) {
  assert(submit_info_id != kInvalidSubmitInfoId);
  std::lock_guard<std::mutex> slock(submit_infos_mutex_);
  if (submit_infos_.find(submit_info_id) != submit_infos_.end()) {
    SubmitInfo& submit_info = submit_infos_[submit_info_id];
    // Update the value of signal semaphores
    auto semaphore_tracker = device_->GetSemaphoreTracker();
    for (size_t i = 0; i < submit_info.signal_semaphores.size(); i++) {
      semaphore_tracker->WriteMarker(
          submit_info.signal_semaphores[i], vk_command_buffer,
          VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
          submit_info.signal_semaphore_values[i],
          {SemaphoreModifierType::kModifierQueueSubmit, qsubmit_id});
    }
    // Write the state of the submit
    device_->CmdWriteBufferMarkerAMD(
        vk_command_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        submit_info.bottom_marker.buffer, submit_info.bottom_marker.offset,
        SubmitState::kFinished);
  } else {
    std::cerr << "GFR Warning: No previous record of queued submit in submit "
                 "tracker."
              << std::endl;
  }
}

void SubmitTracker::CleanupSubmitInfos() {
  std::lock_guard<std::mutex> qlock(queue_submits_mutex_);
  std::lock_guard<std::mutex> slock(submit_infos_mutex_);
  for (auto qsubmit_it = queue_submits_.begin();
       qsubmit_it != queue_submits_.end();) {
    auto& queue_submit_info_ids = qsubmit_it->second;
    for (auto submit_it = queue_submit_info_ids.begin();
         submit_it != queue_submit_info_ids.end();) {
      auto submit_info_id = *submit_it;
      auto it = submit_infos_.find(submit_info_id);
      if (it == submit_infos_.end()) {
        std::cerr << "GFR Warning: No previous record of queued submit in "
                     "submit tracker: "
                  << submit_info_id << std::endl;
        submit_it++;
        continue;
      }
      const SubmitInfo& submit_info = it->second;
      auto submit_status =
          *(uint32_t*)(submit_info.bottom_marker.cpu_mapped_address);
      if (submit_status == SubmitState::kFinished) {
        // Free extra command buffers used to track the state of the submit and
        // the values of the semaphores
        std::vector<VkCommandBuffer> tracking_buffers{
            submit_info.start_marker_cb, submit_info.end_marker_cb};
        device_->FreeCommandBuffers(submit_info.helper_cbs_command_pool, 2,
                                    tracking_buffers.data());
        submit_infos_.erase(it);
        submit_it = queue_submit_info_ids.erase(submit_it);
      } else {
        submit_it++;
      }
    }
    if (queue_submit_info_ids.size() == 0) {
      qsubmit_it = queue_submits_.erase(qsubmit_it);
    } else {
      qsubmit_it++;
    }
  }
}

void SubmitTracker::RecordBindSparseHelperSubmit(
    QueueBindSparseId qbind_sparse_id, const VkSubmitInfo* vk_submit_info,
    VkCommandPool vk_pool) {
  HelperSubmitInfo hsubmit_info;
  // Reserve the marker
  if (!device_->AllocateMarker(&hsubmit_info.marker)) {
    std::cerr
        << "GFR warning: Cannot acquire marker for QueueBindSparse's helper "
           "submit."
        << std::endl;
    return;
  }
  std::lock_guard<std::mutex> lock(helper_submit_infos_mutex_);
  helper_submit_infos_.push_back(hsubmit_info);
  auto& helper_submit_info = helper_submit_infos_.back();
  helper_submit_info.marker_cb = vk_submit_info->pCommandBuffers[0];
  helper_submit_info.command_pool = vk_pool;

  // Write the state of the submit
  *(uint32_t*)(helper_submit_info.marker.cpu_mapped_address) =
      SubmitState::kQueued;

  device_->CmdWriteBufferMarkerAMD(
      helper_submit_info.marker_cb, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      helper_submit_info.marker.buffer, helper_submit_info.marker.offset,
      SubmitState::kFinished);

  // Extract signal semaphore values from submit info
  std::unordered_map<VkSemaphore, uint64_t /*signal_value*/> signal_semaphores;
  for (uint32_t i = 0; i < vk_submit_info->signalSemaphoreCount; i++) {
    signal_semaphores[vk_submit_info->pSignalSemaphores[i]] = 1;
  }

  auto semaphore_tracker = device_->GetSemaphoreTracker();
  const VkTimelineSemaphoreSubmitInfoKHR* timeline_semaphore_info =
      FindOnChain<VkTimelineSemaphoreSubmitInfoKHR,
                  VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR>(
          vk_submit_info->pNext);
  if (timeline_semaphore_info) {
    for (uint32_t i = 0; i < timeline_semaphore_info->signalSemaphoreValueCount;
         i++) {
      if (semaphore_tracker->GetSemaphoreType(
              vk_submit_info->pSignalSemaphores[i]) ==
          VK_SEMAPHORE_TYPE_TIMELINE_KHR) {
        signal_semaphores[vk_submit_info->pSignalSemaphores[i]] =
            timeline_semaphore_info->pSignalSemaphoreValues[i];
      }
    }
  }

  // Update the value of signal semaphores
  for (auto& it : signal_semaphores) {
    semaphore_tracker->WriteMarker(
        it.first, helper_submit_info.marker_cb,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, it.second,
        {SemaphoreModifierType::kModifierQueueBindSparse, qbind_sparse_id});
  }

  // Reset binary wait semaphores
  for (size_t i = 0; i < vk_submit_info->waitSemaphoreCount; i++) {
    if (semaphore_tracker->GetSemaphoreType(
            vk_submit_info->pWaitSemaphores[i]) ==
        VK_SEMAPHORE_TYPE_BINARY_KHR) {
      semaphore_tracker->WriteMarker(
          vk_submit_info->pWaitSemaphores[i], helper_submit_info.marker_cb,
          VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0,
          {SemaphoreModifierType::kModifierQueueBindSparse, qbind_sparse_id});
    }
  }
}

void SubmitTracker::CleanupBindSparseHelperSubmits() {
  std::lock_guard<std::mutex> lock(helper_submit_infos_mutex_);
  for (auto helper_submit_info = helper_submit_infos_.begin();
       helper_submit_info != helper_submit_infos_.end();) {
    auto submit_status =
        *(uint32_t*)(helper_submit_info->marker.cpu_mapped_address);
    if (submit_status == SubmitState::kFinished) {
      // Free the command buffer used to track the state of the submit and
      // the values of the semaphores
      device_->FreeCommandBuffers(helper_submit_info->command_pool, 1,
                                  &(helper_submit_info->marker_cb));
      device_->FreeMarker(helper_submit_info->marker);
      helper_submit_info = helper_submit_infos_.erase(helper_submit_info);
    } else {
      helper_submit_info++;
    }
  }
}

bool SubmitTracker::QueuedSubmitWaitingOnSemaphores(
    SubmitInfoId submit_info_id) const {
  auto it = submit_infos_.find(submit_info_id);
  const SubmitInfo& submit_info = it->second;
  uint64_t semaphore_value = 0;
  bool current_value_available = false;
  auto semaphore_tracker = device_->GetSemaphoreTracker();
  for (uint32_t i = 0; i < submit_info.wait_semaphores.size(); i++) {
    current_value_available = semaphore_tracker->GetSemaphoreValue(
        submit_info.wait_semaphores[i], semaphore_value);
    if (current_value_available &&
        submit_info.wait_semaphore_values[i] > semaphore_value)
      return true;
  }
  return false;
}

std::vector<TrackedSemaphoreInfo> SubmitTracker::GetTrackedSemaphoreInfos(
    SubmitInfoId submit_info_id, SemaphoreOperation operation) const {
  std::vector<TrackedSemaphoreInfo> tracked_semaphores;
  auto it = submit_infos_.find(submit_info_id);
  if (it == submit_infos_.end()) {
    return tracked_semaphores;
  }
  const SubmitInfo& submit_info = it->second;
  auto semaphore_tracker = device_->GetSemaphoreTracker();
  if (operation == SemaphoreOperation::kWaitOperation) {
    return semaphore_tracker->GetTrackedSemaphoreInfos(
        submit_info.wait_semaphores, submit_info.wait_semaphore_values);
  }
  return semaphore_tracker->GetTrackedSemaphoreInfos(
      submit_info.signal_semaphores, submit_info.signal_semaphore_values);
}

bool SubmitTracker::SubmitInfoHasSemaphores(SubmitInfoId submit_info_id) const {
  std::lock_guard<std::mutex> lock(submit_infos_mutex_);
  auto it = submit_infos_.find(submit_info_id);
  if (it == submit_infos_.end()) {
    return false;
  }
  const SubmitInfo& submit_info = it->second;
  return (submit_info.wait_semaphores.size() > 0 ||
          submit_info.signal_semaphores.size() > 0);
}

std::string SubmitTracker::GetSubmitInfoSemaphoresLog(
    VkDevice vk_device, VkQueue vk_queue, SubmitInfoId submit_info_id) const {
  std::lock_guard<std::mutex> lock(submit_infos_mutex_);
  std::stringstream log;
  log << "[GFR] VkSubmitInfo with semaphores submitted to queue.\n"
      << "[GFR]\tVkDevice: " << device_->GetObjectName((uint64_t)vk_device)
      << ", VkQueue: " << device_->GetObjectName((uint64_t)vk_queue)
      << ", SubmitInfoId: " << submit_info_id << std::endl;
  const char* tab = "[GFR]\t";
  auto wait_semaphores =
      GetTrackedSemaphoreInfos(submit_info_id, kWaitOperation);
  if (wait_semaphores.size() > 0) {
    log << tab << "*** Wait Semaphores ***\n";
    log << device_->GetSemaphoreTracker()->PrintTrackedSemaphoreInfos(
        wait_semaphores, tab);
  }
  auto signal_semaphores =
      GetTrackedSemaphoreInfos(submit_info_id, kSignalOperation);
  if (signal_semaphores.size() > 0) {
    log << tab << "*** Signal Semaphores ***\n";
    log << device_->GetSemaphoreTracker()->PrintTrackedSemaphoreInfos(
        signal_semaphores, tab);
  }
  return log.str();
}

void SubmitTracker::DumpWaitingSubmits(std::ostream& os) {
  std::lock_guard<std::mutex> slock(submit_infos_mutex_);
  if (submit_infos_.size() == 0) {
    return;
  }

  std::string indents[16];
  for (size_t i = 0; i < 16; i++) {
    indents[i] = "\n" + std::string((i + 1) * 2, ' ');
  }

  std::lock_guard<std::mutex> qlock(queue_submits_mutex_);
  os << "\nIncompleteQueueSubmits:";
  size_t index = 0;
  for (const auto& qit : queue_submits_) {
    uint32_t incomplete_submission_counter = 0;
    for (auto& submit_info_index : qit.second) {
      SubmitInfo& submit_info = submit_infos_[submit_info_index];
      // Check submit state
      auto submit_state =
          *(uint32_t*)(submit_info.bottom_marker.cpu_mapped_address);
      if (submit_state == SubmitState::kFinished) continue;
      submit_state = *(uint32_t*)(submit_info.top_marker.cpu_mapped_address);
      if (submit_state == SubmitState::kFinished) continue;
      if (incomplete_submission_counter++ == 0) {
        index = 0;
        os << indents[index] << "-";
        os << indents[++index] << "id: " << qit.first;
      } else {
        index = 1;
      }
      os << indents[index] << "SubmitInfos:";
      os << indents[++index] << "-";
      os << indents[++index] << "id: " << submit_info.submit_info_id;
      os << indents[index] << "state: ";
      if (submit_state == SubmitState::kRunning) {
        os << "[STARTED,INCOMPLETE]";
      } else if (submit_state == SubmitState::kQueued) {
        if (QueuedSubmitWaitingOnSemaphores(submit_info.submit_info_id)) {
          os << "[QUEUED,WAITING_ON_SEMAPHORES]";
        } else {
          os << "[QUEUED,NOT_WAITING_ON_SEMAPHORES]";
        }
        auto wait_semaphores = GetTrackedSemaphoreInfos(
            submit_info.submit_info_id, kWaitOperation);
        if (wait_semaphores.size() > 0) {
          os << indents[index] << "WaitSemaphores:";
          for (auto it = wait_semaphores.begin(); it != wait_semaphores.end();
               it++) {
            auto lindex = index;
            os << indents[++lindex] << "-";
            os << indents[++lindex];
            os << device_->GetObjectInfo((uint64_t)it->semaphore,
                                         indents[lindex])
               << indents[lindex] << "type: ";
            if (it->semaphore_type == VK_SEMAPHORE_TYPE_BINARY_KHR)
              os << "Binary";
            else
              os << "Timeline";
            os << indents[lindex]
               << "waitValue: " << it->semaphore_operation_value
               << indents[lindex] << "lastValue: ";
            if (it->current_value_available) {
              os << it->current_value;
            }
          }
        }
        auto signal_semaphores = GetTrackedSemaphoreInfos(
            submit_info.submit_info_id, kSignalOperation);
        if (signal_semaphores.size() > 0) {
          os << indents[index] << "SignalSemaphores:";
          for (auto it = signal_semaphores.begin();
               it != signal_semaphores.end(); it++) {
            auto lindex = index;
            os << indents[++lindex] << "-";
            os << indents[++lindex];
            os << "vkSemaphore: "
               << device_->GetObjectInfo((uint64_t)it->semaphore,
                                         indents[lindex])
               << indents[lindex] << "type: ";
            if (it->semaphore_type == VK_SEMAPHORE_TYPE_BINARY_KHR)
              os << "Binary";
            else
              os << "Timeline";
            os << indents[lindex]
               << "signalValue: " << it->semaphore_operation_value;
          }
        }
      }
    }
  }
}

}  // namespace gfr
