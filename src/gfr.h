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

#ifndef GFR_H
#define GFR_H

#include <vulkan/vulkan.h>

#ifdef __linux__
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include <atomic>
#include <cassert>
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "bind_sparse_utils.h"
#include "command.h"
#include "command_buffer_tracker.h"
#include "device.h"
#include "layer_base.h"
#include "submit_tracker.h"

namespace GFR {

using StringArray = std::vector<std::string>;

struct DeviceCreateInfo {
  VkDeviceCreateInfo original_create_info;
  VkDeviceCreateInfo modified_create_info;
};

enum QueueOperationType {
  kQueueSubmit,
  kQueueBindSparse,
};

enum CrashSource {
  kDeviceLostError,
  kHangDaemon,
  kWatchdogTimer,
};

// Original bind sparse info with the submit tracker that tracks semaphores for
// the respective device.
struct PackedBindSparseInfo {
  const VkQueue queue;
  const uint32_t bind_info_count;
  const VkBindSparseInfo* bind_infos;
  SemaphoreTracker* semaphore_tracker;

  PackedBindSparseInfo(VkQueue queue_, uint32_t bind_info_count_,
                       const VkBindSparseInfo* bind_infos_)
      : queue(queue_),
        bind_info_count(bind_info_count_),
        bind_infos(bind_infos_){};
};

// Expanded bind sparse info, including all the information needed to correctly
// insert semaphore tracking VkSubmitInfos between vkQueueBindSparse calls.
struct ExpandedBindSparseInfo {
  // Input: original bind sparse info.
  const PackedBindSparseInfo* packed_bind_sparse_info;
  // Vector of queue operation types, used to control interleaving order.
  std::vector<QueueOperationType> queue_operation_types;
  // Vector of submit info structs to be submitted to the queue.
  std::vector<VkSubmitInfo> submit_infos;
  // Vector of bool, specifying if a submit info includes a signal operation on
  // a timeline semaphore.
  std::vector<bool> has_timeline_semaphore_info;
  // Place holder for timeline semaphore infos used in queue submit infos.
  std::vector<VkTimelineSemaphoreSubmitInfoKHR> timeline_semaphore_infos;
  // Place holder for vectors of binary semaphores used in a wait semaphore
  // operation in a bind sparse info. This is needed since we need to signal
  // these semaphores in the same vkQueueSubmit that we consume them for
  // tracking (so the bind sparse info which is the real consumer of the
  // semaphore can proceed).
  std::vector<std::vector<VkSemaphore>> wait_binary_semaphores;

  ExpandedBindSparseInfo(PackedBindSparseInfo* packed_bind_sparse_info_)
      : packed_bind_sparse_info(packed_bind_sparse_info_){};
};

static inline void GfrNewHandler() {
  std::cout << "GFR: Memory allocation failed!" << std::endl;
  std::cerr << "GFR: Memory allocation failed!" << std::endl;
  std::set_new_handler(nullptr);
}

template <typename T, typename... Args>
T* GfrNew(Args&&... args) {
  std::set_new_handler(GfrNewHandler);
  return new T(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
T* GfrNewArray(size_t size) {
  std::set_new_handler(GfrNewHandler);
  return new T[size];
}

class GfrContext {
 public:
  GfrContext();
  virtual ~GfrContext();

  VkInstance GetInstance() { return vk_instance_; }

  void MakeOutputPath();
  const std::string& GetOutputPath() const;

  const ShaderModule* FindShaderModule(VkShaderModule shader) const;

  bool DumpShadersOnCrash() const;
  bool DumpShadersOnBind() const;

  bool TrackingSemaphores() { return track_semaphores_; };
  bool TracingAllSemaphores() { return trace_all_semaphores_; };
  QueueSubmitId GetNextQueueSubmitId() { return ++queue_submit_index_; };
  VkCommandPool GetHelperCommandPool(VkDevice vk_device, VkQueue queue);
  SubmitInfoId RegisterSubmitInfo(VkDevice vk_device,
                                  QueueSubmitId queue_submit_id,
                                  const VkSubmitInfo* vk_submit_info);
  void LogSubmitInfoSemaphores(VkDevice vk_device, VkQueue vk_queue,
                               SubmitInfoId submit_info_id);
  void StoreSubmitHelperCommandBuffersInfo(VkDevice vk_device,
                                           SubmitInfoId submit_info_id,
                                           VkCommandPool vk_pool,
                                           VkCommandBuffer start_marker_cb,
                                           VkCommandBuffer end_marker_cb);

  void RecordSubmitStart(VkDevice vk_device, QueueSubmitId qsubmit_id,
                         SubmitInfoId submit_info_id,
                         VkCommandBuffer vk_command_buffer);

  void RecordSubmitFinish(VkDevice vk_device, QueueSubmitId qsubmit_id,
                          SubmitInfoId submit_info_id,
                          VkCommandBuffer vk_command_buffer);

  QueueBindSparseId GetNextQueueBindSparseId() {
    return ++queue_bind_sparse_index_;
  };

  void RecordBindSparseHelperSubmit(VkDevice vk_device,
                                    QueueBindSparseId qbind_sparse_id,
                                    const VkSubmitInfo* vk_submit_info,
                                    VkCommandPool vk_pool);

  VkDevice GetQueueDevice(VkQueue queue) const;
  bool ShouldExpandQueueBindSparseToTrackSemaphores(
      PackedBindSparseInfo* packed_bind_sparse_info);
  void ExpandBindSparseInfo(ExpandedBindSparseInfo* bind_sparse_expand_info);
  void LogBindSparseInfosSemaphores(VkQueue vk_queue, uint32_t bind_info_count,
                                    const VkBindSparseInfo* bind_infos);

 private:
  void AddObjectInfo(VkDevice device, uint64_t handle, ObjectInfoPtr info);
  std::string GetObjectName(VkDevice vk_device, uint64_t handle);
  std::string GetObjectInfo(VkDevice vk_device, uint64_t handle);

  void DumpAllDevicesExecutionState(CrashSource crash_source);
  void DumpDeviceExecutionState(VkDevice vk_device, bool dump_prologue,
                                CrashSource crash_source, std::ostream* os);
  void DumpDeviceExecutionState(const Device* device, bool dump_prologue,
                                CrashSource crash_source, std::ostream* os);
  void DumpDeviceExecutionState(const Device* device, std::string error_report,
                                bool dump_prologue, CrashSource crash_source,
                                std::ostream* os);
  void DumpDeviceExecutionStateValidationFailed(const Device* device,
                                                std::ostream& os);

  void DumpReportPrologue(std::ostream& os, const Device* device);
  void WriteReport(std::ostream& os, CrashSource crash_source);

  void StartWatchdogTimer();
  void StopWatchdogTimer();
  void WatchdogTimer();

  void StartGpuHangdListener();
  void StopGpuHangdListener();
  void GpuHangdListener();

  void ValidateCommandBufferNotInUse(CommandBuffer* commandBuffer);
  void DumpCommandBufferState(CommandBuffer* p_cmd);

 public:
  void PreApiFunction(const char* api_name);
  void PostApiFunction(const char* api_name);

  const VkInstanceCreateInfo* GetModifiedInstanceCreateInfo(
      const VkInstanceCreateInfo* pCreateInfo);
  const VkDeviceCreateInfo* GetModifiedDeviceCreateInfo(
      VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo);

  void PreDestroyBuffer(VkDevice device, VkBuffer buffer,
                        const VkAllocationCallbacks* pAllocator);
  void PostDestroyBuffer(VkDevice device, VkBuffer buffer,
                         const VkAllocationCallbacks* pAllocator);

  VkResult PreCreateCommandPool(VkDevice device,
                                VkCommandPoolCreateInfo const* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkCommandPool* pCommandPool);
  VkResult PostCreateCommandPool(VkDevice device,
                                 VkCommandPoolCreateInfo const* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator,
                                 VkCommandPool* pCommandPool, VkResult result);
  void PreDestroyCommandPool(VkDevice device, VkCommandPool commandPool,
                             const VkAllocationCallbacks* pAllocator);
  void PostDestroyCommandPool(VkDevice device, VkCommandPool commandPool,
                              const VkAllocationCallbacks* pAllocator);
  VkResult PreResetCommandPool(VkDevice device, VkCommandPool commandPool,
                               VkCommandPoolResetFlags flags);
  VkResult PostResetCommandPool(VkDevice device, VkCommandPool commandPool,
                                VkCommandPoolResetFlags flags, VkResult result);

  VkResult PreAllocateCommandBuffers(
      VkDevice device, VkCommandBufferAllocateInfo const* pAllocateInfo,
      VkCommandBuffer* pCommandBuffers);
  VkResult PostAllocateCommandBuffers(
      VkDevice device, VkCommandBufferAllocateInfo const* pAllocateInfo,
      VkCommandBuffer* pCommandBuffers, VkResult result);
  void PreFreeCommandBuffers(VkDevice device, VkCommandPool commandPool,
                             uint32_t commandBufferCount,
                             VkCommandBuffer const* pCommandBuffers);
  void PostFreeCommandBuffers(VkDevice device, VkCommandPool commandPool,
                              uint32_t commandBufferCount,
                              VkCommandBuffer const* pCommandBuffers);

  void PreUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount,
                               VkWriteDescriptorSet const* pDescriptorWrites,
                               uint32_t descriptorCopyCount,
                               VkCopyDescriptorSet const* pDescriptorCopies);
  void PostUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount,
                                VkWriteDescriptorSet const* pDescriptorWrites,
                                uint32_t descriptorCopyCount,
                                VkCopyDescriptorSet const* pDescriptorCopies);

  VkResult PostCreateDevice(VkPhysicalDevice physicalDevice,
                            VkDeviceCreateInfo const* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator,
                            VkDevice* pDevice, VkResult result);
  void PreDestroyDevice(VkDevice device,
                        const VkAllocationCallbacks* pAllocator);
  void PostDestroyDevice(VkDevice device,
                         const VkAllocationCallbacks* pAllocator);
  VkResult PostDeviceWaitIdle(VkDevice device, VkResult result);

  VkResult PreCreateInstance(VkInstanceCreateInfo const* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkInstance* pInstance);
  VkResult PostCreateInstance(VkInstanceCreateInfo const* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator,
                              VkInstance* pInstance, VkResult result);
  void PreDestroyInstance(VkInstance instance,
                          const VkAllocationCallbacks* pAllocator);
  void PostDestroyInstance(VkInstance instance,
                           const VkAllocationCallbacks* pAllocator);

  VkResult PreCreateGraphicsPipelines(
      VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
      VkGraphicsPipelineCreateInfo const* pCreateInfos,
      const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
  VkResult PostCreateGraphicsPipelines(
      VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
      VkGraphicsPipelineCreateInfo const* pCreateInfos,
      const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines,
      VkResult result);
  VkResult PreCreateComputePipelines(
      VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
      VkComputePipelineCreateInfo const* pCreateInfos,
      const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
  VkResult PostCreateComputePipelines(
      VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
      VkComputePipelineCreateInfo const* pCreateInfos,
      const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines,
      VkResult result);
  void PreDestroyPipeline(VkDevice device, VkPipeline pipeline,
                          const VkAllocationCallbacks* pAllocator);
  void PostDestroyPipeline(VkDevice device, VkPipeline pipeline,
                           const VkAllocationCallbacks* pAllocator);
  VkResult PreCreateShaderModule(VkDevice device,
                                 VkShaderModuleCreateInfo const* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator,
                                 VkShaderModule* pShaderModule);
  VkResult PostCreateShaderModule(VkDevice device,
                                  VkShaderModuleCreateInfo const* pCreateInfo,
                                  const VkAllocationCallbacks* pAllocator,
                                  VkShaderModule* pShaderModule,
                                  VkResult result);
  void PreDestroyShaderModule(VkDevice device, VkShaderModule shaderModule,
                              const VkAllocationCallbacks* pAllocator);
  void PostDestroyShaderModule(VkDevice device, VkShaderModule shaderModule,
                               const VkAllocationCallbacks* pAllocator);

  void PostGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex,
                          uint32_t queueIndex, VkQueue* pQueue);
  VkResult PreQueueSubmit(VkQueue queue, uint32_t submitCount,
                          VkSubmitInfo const* pSubmits, VkFence fence);
  VkResult PostQueueSubmit(VkQueue queue, uint32_t submitCount,
                           VkSubmitInfo const* pSubmits, VkFence fence,
                           VkResult result);
  VkResult PostQueueWaitIdle(VkQueue queue, VkResult result);
  VkResult PostQueueBindSparse(VkQueue queue, uint32_t bindInfoCount,
                               VkBindSparseInfo const* pBindInfo, VkFence fence,
                               VkResult result);

  VkResult PostQueuePresentKHR(VkQueue queue,
                               VkPresentInfoKHR const* pPresentInfo,
                               VkResult result);

  VkResult PostGetQueryPoolResults(VkDevice device, VkQueryPool queryPool,
                                   uint32_t firstQuery, uint32_t queryCount,
                                   size_t dataSize, void* pData,
                                   VkDeviceSize stride,
                                   VkQueryResultFlags flags, VkResult result);
  VkResult PostWaitForFences(VkDevice device, uint32_t fenceCount,
                             VkFence const* pFences, VkBool32 waitAll,
                             uint64_t timeout, VkResult result);
  VkResult PostAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain,
                                   uint64_t timeout, VkSemaphore semaphore,
                                   VkFence fence, uint32_t* pImageIndex,
                                   VkResult result);
  VkResult PostGetFenceStatus(VkDevice device, VkFence fence, VkResult result);

  VkResult PreDebugMarkerSetObjectNameEXT(
      VkDevice device, VkDebugMarkerObjectNameInfoEXT const* pNameInfo);
  VkResult PostDebugMarkerSetObjectNameEXT(
      VkDevice device, VkDebugMarkerObjectNameInfoEXT const* pNameInfo,
      VkResult result);

  VkResult PreSetDebugUtilsObjectNameEXT(
      VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
  VkResult PostSetDebugUtilsObjectNameEXT(
      VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo,
      VkResult result);

  VkResult PostCreateSemaphore(VkDevice device,
                               VkSemaphoreCreateInfo const* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkSemaphore* pSemaphore, VkResult result);
  void PostDestroySemaphore(VkDevice device, VkSemaphore semaphore,
                            const VkAllocationCallbacks* pAllocator);
  VkResult PostSignalSemaphoreKHR(VkDevice device,
                                  const VkSemaphoreSignalInfoKHR* pSignalInfo,
                                  VkResult result);
  VkResult PreWaitSemaphoresKHR(VkDevice device,
                                const VkSemaphoreWaitInfoKHR* pWaitInfo,
                                uint64_t timeout);
  VkResult PostWaitSemaphoresKHR(VkDevice device,
                                 const VkSemaphoreWaitInfoKHR* pWaitInfo,
                                 uint64_t timeout, VkResult result);
  VkResult PostGetSemaphoreCounterValueKHR(VkDevice device,
                                           VkSemaphore semaphore,
                                           uint64_t* pValue, VkResult result);

#include "gfr_commands.h.inc"

 private:
  using CStringArray = std::vector<char*>;

  StringArray instance_extension_names_;
  StringArray instance_extension_names_original_;
  CStringArray instance_extension_names_cstr_;
  VkInstance vk_instance_ = VK_NULL_HANDLE;

  InstanceDispatchTable instance_dispatch_table_;
  VkInstanceCreateInfo instance_create_info_;

  mutable std::mutex device_create_infos_mutex_;
  std::unordered_map<const VkDeviceCreateInfo* /*modified_create_info*/,
                     std::unique_ptr<DeviceCreateInfo>>
      device_create_infos_;

  struct ApplicationInfo {
    std::string applicationName;
    uint32_t applicationVersion;

    std::string engineName;
    uint32_t engineVersion;
    uint32_t apiVersion;
  };

  std::unique_ptr<ApplicationInfo> application_info_;

  mutable std::mutex devices_mutex_;
  std::unordered_map<VkDevice, DevicePtr> devices_;
  StringArray device_extension_names_;
  StringArray device_extension_names_original_;
  CStringArray device_extension_names_cstr_;
  CStringArray device_extension_names_original_cstr_;

  // Tracks VkDevice that a VkQueue belongs to. This is needed when tracking
  // semaphores in vkQueueBindSparse, for which we need to allocate new command
  // buffers from the device that owns the queue. This is valid since VkQueue is
  // an opaque handle, since guaranteed to be unique.
  mutable std::mutex queue_device_tracker_mutex_;
  std::unordered_map<VkQueue, VkDevice> queue_device_tracker_;

  // Debug flags
  bool debug_dump_on_begin_ = false;
  int debug_autodump_rate_ = 0;
  bool debug_dump_all_command_buffers_ = false;
  bool debug_dump_shaders_on_crash_ = false;
  bool debug_dump_shaders_on_bind_ = false;

  int shader_module_load_options_ = ShaderModule::LoadOptions::kNone;

  bool instrument_all_commands_ = false;
  bool validate_command_buffer_state_ = true;
  bool track_semaphores_ = false;
  bool trace_all_semaphores_ = false;

  // TODO(aellem) some verbosity/trace modes?
  bool trace_all_ = false;

  bool output_path_created_ = false;
  std::string base_output_path_;
  std::string output_path_;
  std::string output_name_;

  bool log_configs_ = false;
  StringArray configs_;
  template <class T>
  void GetEnvVal(const char* name, T* value);

  int total_submits_ = 0;
  int total_logs_ = 0;

  QueueSubmitId queue_submit_index_ = 0;
  QueueBindSparseId queue_bind_sparse_index_ = 0;

  // Watchdog
  // TODO(aellem) we should have a way to shut this down, but currently the
  // GFR context never gets destroyed
  std::unique_ptr<std::thread> watchdog_thread_;
  std::atomic<bool> watchdog_running_;
  std::atomic<long long> last_submit_time_;
  uint64_t watchdog_timer_ms_ = 0;

// Hang daemon listener thread.
#ifdef __linux__
  std::unique_ptr<std::thread> gpuhangd_thread_;
  int gpuhangd_socket_ = -1;
  int gpuhang_event_id_ = 0;
#endif  // __linux__
};

}  // namespace GFR

#endif  // GFR_H
