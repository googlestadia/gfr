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

#ifndef GFR_BIND_SPARSE_UTILS_H
#define GFR_BIND_SPARSE_UTILS_H

#include <vulkan/vulkan.h>

#include <vector>

#include "device.h"
#include "gfr.h"
#include "semaphore_tracker.h"

namespace GFR {

class Device;
class SemaphoreTracker;
struct PackedBindSparseInfo;
struct ExpandedBindSparseInfo;

class BindSparseUtils {
 public:
  static bool ShouldExpandQueueBindSparseToTrackSemaphores(
      const PackedBindSparseInfo* packed_bind_sparse_info);

  static void ExpandBindSparseInfo(
      ExpandedBindSparseInfo* bind_sparse_expand_info);

  static std::string LogBindSparseInfosSemaphores(
      const Device* device, VkDevice vk_device, VkQueue vk_queue,
      uint32_t bind_info_count, const VkBindSparseInfo* bind_infos);

 private:
  static bool BindSparseInfoWaitsOnBinarySemaphores(
      const VkBindSparseInfo* bind_infos,
      const SemaphoreTracker* semaphore_tracker);

  static void GetWaitBinarySemaphores(
      const VkBindSparseInfo* bind_info,
      const SemaphoreTracker* semaphore_tracker,
      std::vector<VkSemaphore>* wait_binary_semaphores);
};

}  // namespace GFR

#endif  // GFR_BIND_SPARSE_UTILS_H
