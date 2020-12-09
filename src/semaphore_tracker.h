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

#ifndef GFR_SEMAPHORE_TRACKER_H
#define GFR_SEMAPHORE_TRACKER_H

#include <vulkan/vulkan.h>

#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "marker.h"

namespace gfr {

class Device;

enum SemaphoreOperation { kWaitOperation, kSignalOperation };

enum SemaphoreModifierType {
  kNotModified = 0,
  kModifierHost = 1,
  kModifierQueueSubmit = 2,
  kModifierQueueBindSparse = 3
};

struct SemaphoreModifierInfo {
  SemaphoreModifierType type = kNotModified;
  uint32_t id = 0;
};

struct TrackedSemaphoreInfo {
  VkSemaphore semaphore = VK_NULL_HANDLE;
  VkSemaphoreTypeKHR semaphore_type = VK_SEMAPHORE_TYPE_BINARY_KHR;
  // This can be a wait operation value or a signal operation value.
  uint64_t semaphore_operation_value = 0;
  uint64_t current_value = 0;
  bool current_value_available = false;
  SemaphoreModifierInfo last_modifier;
};

class SemaphoreTracker {
 public:
  SemaphoreTracker(Device* device, bool track_semaphores_last_setter);

  void RegisterSemaphore(VkSemaphore vk_semaphore, VkSemaphoreTypeKHR type,
                         uint64_t value);
  void SignalSemaphore(VkSemaphore vk_semaphore, uint64_t value,
                       SemaphoreModifierInfo modifier_info);
  void EraseSemaphore(VkSemaphore vk_semaphore);

  bool GetSemaphoreValue(VkSemaphore vk_semaphore, uint64_t& value) const;
  VkSemaphoreTypeKHR GetSemaphoreType(VkSemaphore vk_semaphore) const;

  void BeginWaitOnSemaphores(int pid, int tid,
                             const VkSemaphoreWaitInfoKHR* pWaitInfo);
  void EndWaitOnSemaphores(int pid, int tid,
                           const VkSemaphoreWaitInfoKHR* pWaitInfo);
  void DumpWaitingThreads(std::ostream& os);

  void WriteMarker(VkSemaphore vk_semaphore, VkCommandBuffer vk_command_buffer,
                   VkPipelineStageFlagBits vk_pipeline_stage, uint64_t value,
                   SemaphoreModifierInfo modifier_info);

  std::vector<TrackedSemaphoreInfo> GetTrackedSemaphoreInfos(
      const std::vector<VkSemaphore>& semaphores,
      const std::vector<uint64_t>& semaphore_values);

  std::string PrintTrackedSemaphoreInfos(
      const std::vector<TrackedSemaphoreInfo>& tracked_semaphores,
      const char* tab) const;

 private:
  Device* device_ = nullptr;
  bool track_semaphores_last_setter_ = false;

  struct SemaphoreInfo {
    // VkSemaphore used as the key in the container, so not included here.
    VkSemaphoreTypeKHR semaphore_type = VK_SEMAPHORE_TYPE_BINARY_KHR;
    Marker marker, last_modifier_marker;
    SemaphoreInfo() {
      marker.type = MarkerType::kUint64;
      last_modifier_marker.type = MarkerType::kUint64;
    }
    uint64_t EncodeModifierInfo(SemaphoreModifierInfo modifier_info) {
      return ((uint64_t)modifier_info.type << 32) | modifier_info.id;
    }
    void UpdateLastModifier(SemaphoreModifierInfo modifier_info) {
      *(uint64_t*)(last_modifier_marker.cpu_mapped_address) =
          EncodeModifierInfo(modifier_info);
    }
    SemaphoreModifierInfo GetLastModifier() {
      SemaphoreModifierInfo last_modifier;
      uint64_t last_modifier_64 =
          *(uint64_t*)(last_modifier_marker.cpu_mapped_address);
      last_modifier.type =
          (SemaphoreModifierType)(last_modifier_64 & 0xffffffff);
      last_modifier.id = last_modifier_64 >> 32;
      return last_modifier;
    }
  };

  mutable std::mutex semaphores_mutex_;
  std::unordered_map<VkSemaphore, SemaphoreInfo> semaphores_;

  enum SemaphoreWaitType {
    kAll = 0,
    kAny = 1,
  };

  struct WaitingThreadInfo {
    int pid = 0;
    int tid = 0;
    SemaphoreWaitType wait_type;
    std::vector<VkSemaphore> semaphores;
    std::vector<uint64_t> wait_values;
  };

  mutable std::mutex waiting_threads_mutex_;
  std::vector<WaitingThreadInfo> waiting_threads_;
};

using SemaphoreTrackerPtr = std::unique_ptr<SemaphoreTracker>;

}  // namespace gfr

#endif  // GFR_SEMAPHORE_TRACKER_H
