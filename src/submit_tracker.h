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

#ifndef GFR_SUBMIT_TRACKER_H
#define GFR_SUBMIT_TRACKER_H

#include <vulkan/vulkan.h>

#include <map>
#include <unordered_map>
#include <vector>

#include "command.h"
#include "command_pool.h"
#include "marker.h"
#include "semaphore_tracker.h"

namespace GFR {

class Device;

const uint32_t kInvalidSubmitInfoId = 0xFFFFFFFF;

// Unique id counter for submit infos
static uint32_t submit_info_counter = 0;

typedef uint32_t SubmitInfoId;
typedef uint32_t QueueSubmitId;
typedef uint32_t QueueBindSparseId;

class SubmitTracker {
 public:
  SubmitTracker(Device* device);

  SubmitInfoId RegisterSubmitInfo(QueueSubmitId queue_submit_index,
                                  const VkSubmitInfo* submit_info);
  void StoreSubmitHelperCommandBuffersInfo(SubmitInfoId submit_info_id,
                                           VkCommandPool vk_pool,
                                           VkCommandBuffer start_marker_cb,
                                           VkCommandBuffer end_marker_cb);
  void RecordSubmitStart(QueueSubmitId qsubmit_id, SubmitInfoId submit_info_id,
                         VkCommandBuffer vk_command_buffer);
  void RecordSubmitFinish(QueueSubmitId qsubmit_id, SubmitInfoId submit_info_id,
                          VkCommandBuffer vk_command_buffer);
  void CleanupSubmitInfos();
  bool SubmitInfoHasSemaphores(SubmitInfoId submit_info_id) const;
  std::string GetSubmitInfoSemaphoresLog(VkDevice vk_device, VkQueue vk_queue,
                                         SubmitInfoId submit_info_id) const;

  void RecordBindSparseHelperSubmit(QueueBindSparseId qbind_sparse_id,
                                    const VkSubmitInfo* vk_submit_info,
                                    VkCommandPool vk_pool);
  void CleanupBindSparseHelperSubmits();

  bool QueuedSubmitWaitingOnSemaphores(SubmitInfoId submit_info_id) const;
  void DumpWaitingSubmits(std::ostream& os);

  std::vector<TrackedSemaphoreInfo> GetTrackedSemaphoreInfos(
      SubmitInfoId submit_info_id, SemaphoreOperation operation) const;

 private:
  Device* device_ = nullptr;

  enum SubmitState {
    kInvalid = 0,
    kQueued = 1,
    kRunning = 2,
    kFinished = 3,
  };

  struct SubmitInfo {
    SubmitInfoId submit_info_id;
    const VkSubmitInfo* vk_submit_info;
    std::vector<VkSemaphore> wait_semaphores;
    std::vector<uint64_t> wait_semaphore_values;
    std::vector<VkPipelineStageFlags> wait_semaphore_pipeline_stages;
    std::vector<VkCommandBuffer> command_buffers;
    std::vector<VkSemaphore> signal_semaphores;
    std::vector<uint64_t> signal_semaphore_values;
    // Markers used to track the state of the submit.
    Marker top_marker;
    Marker bottom_marker;
    // Info for extra command buffers used to track semaphore values
    VkCommandPool helper_cbs_command_pool = VK_NULL_HANDLE;
    VkCommandBuffer start_marker_cb = VK_NULL_HANDLE;
    VkCommandBuffer end_marker_cb = VK_NULL_HANDLE;
    // vkQueueSubmit tracking index
    uint32_t queue_submit_index = 0;
    SubmitInfo();
  };

  mutable std::mutex queue_submits_mutex_;
  std::map<QueueSubmitId, std::vector<SubmitInfoId>> queue_submits_;

  mutable std::mutex submit_infos_mutex_;
  std::map<SubmitInfoId, SubmitInfo> submit_infos_;

  // Helper submit infos used to track signal semaphore operations in
  // vkQueueBindSparse.
  struct HelperSubmitInfo {
    QueueBindSparseId qbind_sparse_id;
    // Marker used to track the state of the helper submit and its signal
    // semaphore operations. While GFR doesn't care about the state of helper
    // submits in its log, this is necessary to free the extra command buffer
    // allocated and used in the helper submit.
    Marker marker;
    // Info for the command buffer used to track semaphore values.
    // We expect helper submit infos to have only one command buffer.
    VkCommandBuffer marker_cb = VK_NULL_HANDLE;
    VkCommandPool command_pool = VK_NULL_HANDLE;

    HelperSubmitInfo() { marker.type = MarkerType::kUint32; }
  };

  std::mutex helper_submit_infos_mutex_;
  std::vector<HelperSubmitInfo> helper_submit_infos_;
};

using SubmitTrackerPtr = std::unique_ptr<SubmitTracker>;

}  // namespace GFR

#endif  // GFR_SUBMIT_TRACKER_H
