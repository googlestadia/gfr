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

#include "semaphore_tracker.h"

#include <cstring>
#include <iomanip>
#include <sstream>

#include "device.h"
#include "gfr.h"
#include "util.h"

namespace GFR {

SemaphoreTracker::SemaphoreTracker(Device* p_device,
                                   bool track_semaphores_last_setter)
    : device_(p_device),
      track_semaphores_last_setter_(track_semaphores_last_setter) {}

void SemaphoreTracker::RegisterSemaphore(VkSemaphore vk_semaphore,
                                         VkSemaphoreTypeKHR type,
                                         uint64_t value) {
  {
    std::lock_guard<std::mutex> lock(semaphores_mutex_);
    semaphores_.erase(vk_semaphore);
  }
  // Create a new semaphore info and add it to the semaphores container
  SemaphoreInfo semaphore_info = {};
  semaphore_info.semaphore_type = type;
  // Reserve a marker to track semaphore value
  if (!device_->AllocateMarker(&semaphore_info.marker)) {
    fprintf(stderr,
            "GFR warning: Cannot acquire marker. Not tracking semaphore %s.\n",
            device_->GetObjectName((uint64_t)vk_semaphore).c_str());
    return;
  }
  if (track_semaphores_last_setter_) {
    if (!device_->AllocateMarker(&semaphore_info.last_modifier_marker)) {
      fprintf(stderr,
              "GFR warning: Cannot acquire modifier tracking marker. Not "
              "tracking semaphore %s.\n",
              device_->GetObjectName((uint64_t)vk_semaphore).c_str());
      return;
    }
  }

  {
    std::lock_guard<std::mutex> lock(semaphores_mutex_);
    semaphores_[vk_semaphore] = semaphore_info;
  }
  device_->GetSemaphoreTracker()->SignalSemaphore(
      vk_semaphore, value, {SemaphoreModifierType::kNotModified});
}

void SemaphoreTracker::SignalSemaphore(VkSemaphore vk_semaphore, uint64_t value,
                                       SemaphoreModifierInfo modifier_info) {
  std::lock_guard<std::mutex> slock(semaphores_mutex_);
  if (semaphores_.find(vk_semaphore) != semaphores_.end()) {
    auto& semaphore_info = semaphores_[vk_semaphore];
    *(uint64_t*)(semaphore_info.marker.cpu_mapped_address) = value;
    if (track_semaphores_last_setter_) {
      semaphore_info.UpdateLastModifier(modifier_info);
    }
  } else {
    fprintf(stderr, "Error: Unknown semaphore signaled: %s\n",
            device_->GetObjectName((uint64_t)vk_semaphore).c_str());
  }
}

void SemaphoreTracker::EraseSemaphore(VkSemaphore vk_semaphore) {
  std::lock_guard<std::mutex> lock(semaphores_mutex_);
  semaphores_.erase(vk_semaphore);
}

void SemaphoreTracker::BeginWaitOnSemaphores(
    int pid, int tid, const VkSemaphoreWaitInfoKHR* pWaitInfo) {
  WaitingThreadInfo waiting_thread_info;
  waiting_thread_info.pid = pid;
  waiting_thread_info.tid = tid;
  waiting_thread_info.wait_type = SemaphoreWaitType::kAll;
  if (pWaitInfo->flags & VK_SEMAPHORE_WAIT_ANY_BIT_KHR) {
    waiting_thread_info.wait_type = SemaphoreWaitType::kAny;
  }
  for (uint32_t i = 0; i < pWaitInfo->semaphoreCount; i++) {
    waiting_thread_info.semaphores.push_back(pWaitInfo->pSemaphores[i]);
    waiting_thread_info.wait_values.push_back(pWaitInfo->pValues[i]);
  }
  std::lock_guard<std::mutex> lock(waiting_threads_mutex_);
  waiting_threads_.push_back(waiting_thread_info);
}

void SemaphoreTracker::EndWaitOnSemaphores(
    int pid, int tid, const VkSemaphoreWaitInfoKHR* pWaitInfo) {
  std::lock_guard<std::mutex> lock(waiting_threads_mutex_);
  for (auto it = waiting_threads_.begin(); it != waiting_threads_.end(); it++) {
    if (it->pid == pid && it->tid == tid) {
      waiting_threads_.erase(it);
      return;
    }
  }
}

bool SemaphoreTracker::GetSemaphoreValue(VkSemaphore vk_semaphore,
                                         uint64_t& value) const {
  std::lock_guard<std::mutex> slock(semaphores_mutex_);
  if (semaphores_.find(vk_semaphore) == semaphores_.end()) return false;
  auto& semaphore_info = semaphores_.find(vk_semaphore)->second;
  value = *(uint64_t*)(semaphore_info.marker.cpu_mapped_address);
  return true;
}

VkSemaphoreTypeKHR SemaphoreTracker::GetSemaphoreType(
    VkSemaphore vk_semaphore) const {
  VkSemaphoreTypeKHR semaphore_type = VK_SEMAPHORE_TYPE_BINARY_KHR;
  std::lock_guard<std::mutex> lock(semaphores_mutex_);
  if (semaphores_.find(vk_semaphore) != semaphores_.end())
    semaphore_type = semaphores_.find(vk_semaphore)->second.semaphore_type;
  return semaphore_type;
}

void SemaphoreTracker::WriteMarker(VkSemaphore vk_semaphore,
                                   VkCommandBuffer vk_command_buffer,
                                   VkPipelineStageFlagBits vk_pipeline_stage,
                                   uint64_t value,
                                   SemaphoreModifierInfo modifier_info) {
  std::lock_guard<std::mutex> slock(semaphores_mutex_);
  if (semaphores_.find(vk_semaphore) == semaphores_.end()) return;
  auto& semaphore_info = semaphores_[vk_semaphore];
  auto& marker = semaphore_info.marker;
  uint32_t u32_value = value & 0xffffffff;
  device_->CmdWriteBufferMarkerAMD(vk_command_buffer, vk_pipeline_stage,
                                   marker.buffer, marker.offset, u32_value);
  u32_value = value >> 32;
  device_->CmdWriteBufferMarkerAMD(vk_command_buffer, vk_pipeline_stage,
                                   marker.buffer,
                                   marker.offset + sizeof(uint32_t), u32_value);

  if (track_semaphores_last_setter_) {
    auto& marker = semaphore_info.last_modifier_marker;
    device_->CmdWriteBufferMarkerAMD(vk_command_buffer, vk_pipeline_stage,
                                     marker.buffer, marker.offset,
                                     modifier_info.type);
    device_->CmdWriteBufferMarkerAMD(
        vk_command_buffer, vk_pipeline_stage, marker.buffer,
        marker.offset + sizeof(uint32_t), modifier_info.id);
  }
}

std::vector<TrackedSemaphoreInfo> SemaphoreTracker::GetTrackedSemaphoreInfos(
    const std::vector<VkSemaphore>& semaphores,
    const std::vector<uint64_t>& semaphore_values) {
  std::vector<TrackedSemaphoreInfo> tracked_semaphores;
  std::lock_guard<std::mutex> lock(semaphores_mutex_);
  for (uint32_t i = 0; i < semaphores.size(); i++) {
    if (semaphores_.find(semaphores[i]) == semaphores_.end()) continue;
    auto semaphore_info = semaphores_[semaphores[i]];
    TrackedSemaphoreInfo tracked_semaphore;
    tracked_semaphore.semaphore = semaphores[i];
    tracked_semaphore.semaphore_type = semaphore_info.semaphore_type;
    tracked_semaphore.semaphore_operation_value = semaphore_values[i];
    tracked_semaphore.current_value =
        *(uint64_t*)(semaphore_info.marker.cpu_mapped_address);
    tracked_semaphore.current_value_available = true;
    if (track_semaphores_last_setter_) {
      tracked_semaphore.last_modifier = semaphore_info.GetLastModifier();
    }
    tracked_semaphores.push_back(tracked_semaphore);
  }
  return tracked_semaphores;
}

std::string SemaphoreTracker::PrintTrackedSemaphoreInfos(
    const std::vector<TrackedSemaphoreInfo>& tracked_semaphores,
    const char* tab) const {
  std::stringstream log;
  log << tab << std::setfill(' ') << std::left << std::setw(24) << "Semaphore"
      << std::left << std::setw(12) << "Type" << std::left << std::setw(18)
      << std::left << std::setw(18) << "Operation value" << std::left
      << std::setw(18) << "Last known value"
      << "Last modifier\n";
  for (auto it = tracked_semaphores.begin(); it != tracked_semaphores.end();
       it++) {
    log << tab << std::left << std::setw(24)
        << device_->GetObjectName((uint64_t)it->semaphore, kPreferDebugName);
    if (it->semaphore_type == VK_SEMAPHORE_TYPE_BINARY_KHR) {
      log << std::setfill(' ') << std::left << std::setw(12) << "Binary";
    } else {
      log << std::setfill(' ') << std::left << std::setw(12) << "Timeline";
    }
    log << std::left << std::setw(18) << it->semaphore_operation_value;
    if (it->current_value_available) {
      log << std::setfill(' ') << std::left << std::setw(18)
          << it->current_value;
    } else {
      log << std::setfill(' ') << std::left << std::setw(18) << "NA";
    }
    switch (it->last_modifier.type) {
      case kModifierHost:
        log << "Host";
        break;
      case kModifierQueueSubmit:
        log << "QueueSubmit, Id: " << it->last_modifier.id;
        break;
      case kModifierQueueBindSparse:
        log << "QueueBindSparse, Id: " << it->last_modifier.id;
        break;
      default:
        log << "Not modified";
    }
    log << "\n";
  }
  return log.str();
}

void SemaphoreTracker::DumpWaitingThreads(std::ostream& os) {
  std::lock_guard<std::mutex> lock(waiting_threads_mutex_);
  if (waiting_threads_.size() == 0) {
    return;
  }

  std::string indents[6];
  for (size_t i = 0; i < 6; i++) {
    indents[i] = "\n" + std::string((i + 1) * 2, ' ');
  }

  size_t index = 0;
  os << "\nWaitingThreads:";
  uint64_t semaphore_value = 0;
  for (auto& it : waiting_threads_) {
    auto lindex = index;
    os << indents[lindex] << "-";
    os << indents[++lindex] << "PID: " << it.pid;
    os << indents[lindex] << "TID: " << it.tid;
    if (it.wait_type == SemaphoreWaitType::kAll) {
      os << indents[lindex] << "waitType: "
         << "WaitForAll";
    } else {
      os << indents[lindex] << "waitType: "
         << "WaitForAny";
    }
    os << indents[lindex] << "WaitSemaphores:";
    for (int i = 0; i < it.semaphores.size(); i++) {
      auto sindex = lindex;
      os << indents[++sindex] << "-";
      os << indents[++sindex];
      os << "vkSemaphore: "
         << device_->GetObjectInfo((uint64_t)it.semaphores[i], indents[sindex])
         << indents[sindex] << "type: Timeline" << indents[sindex]
         << "waitValue: " << it.wait_values[i] << indents[sindex]
         << "lastValue: ";
      if (GetSemaphoreValue(it.semaphores[i], semaphore_value)) {
        os << semaphore_value;
      }
    }
  }
}

}  // namespace GFR
