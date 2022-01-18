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

#ifndef GFR_DEVICE_H
#define GFR_DEVICE_H

#include <vulkan/vulkan.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "command.h"
#include "command_pool.h"
#include "layer_base.h"
#include "marker.h"
#include "object_name_db.h"
#include "pipeline.h"
#include "semaphore_tracker.h"
#include "shader_module.h"
#include "submit_tracker.h"

namespace GFR {

const VkDeviceSize kBufferMarkerEventCount = 1024;
const MarkerType kMarkerType = MarkerType::kUint32;

class GfrContext;
struct DeviceCreateInfo;

// Options when dumping a command buffer to a log file.
typedef uint32_t CommandBufferDumpOptions;
struct CommandBufferDumpOption;

class Device {
 public:
  Device(GfrContext* p_gfr, VkPhysicalDevice vk_gpu, VkDevice vk_device,
         bool has_buffer_marker);
  ~Device();
  void SetDeviceCreateInfo(
      std::unique_ptr<DeviceCreateInfo> device_create_info);

  GfrContext* GetGFR() const;
  VkPhysicalDevice GetVkGpu() const;
  VkDevice GetVkDevice() const;

  SubmitTracker* GetSubmitTracker() const { return submit_tracker_.get(); }
  SemaphoreTracker* GetSemaphoreTracker() const {
    return semaphore_tracker_.get();
  }

  void AddObjectInfo(uint64_t handle, ObjectInfoPtr info);
  void AddExtraInfo(uint64_t handle, ExtraObjectInfo info);
  const ObjectInfoDB& GetObjectInfoDB() const { return object_info_db_; }
  std::string GetObjectName(
      uint64_t handle, HandleDebugNamePreference handle_debug_name_preference =
                           kReportBoth) const;
  std::string GetObjectInfo(uint64_t handle,
                            const std::string& indent = kDefaultIndent) const;
  std::string GetObjectInfoNoHandleTag(
      uint64_t handle, const std::string& indent = kDefaultIndent) const;

  bool HasBufferMarker() const;

  const std::vector<VkQueueFamilyProperties>& GetVkQueueFamilyProperties()
      const;

  VkResult CreateBuffer(VkDeviceSize size, VkBuffer* p_buffer,
                        void** cpu_mapped_address);

  VkResult AcquireMarkerBuffer();

  void CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer,
                               VkPipelineStageFlagBits pipelineStage,
                               VkBuffer dstBuffer, VkDeviceSize dstOffset,
                               uint32_t marker);

  void FreeCommandBuffers(VkCommandPool command_pool,
                          uint32_t command_buffer_count,
                          const VkCommandBuffer* command_buffers);

  void AddCommandBuffer(VkCommandBuffer vk_command_buffer);

  bool ValidateCommandBufferNotInUse(CommandBuffer* p_cmd, std::ostream& os);
  bool ValidateCommandBufferNotInUse(VkCommandBuffer vk_command_buffer,
                                     std::ostream& os);
  void DeleteCommandBuffers(const VkCommandBuffer* vk_cmds, uint32_t cb_count);

  void DumpCommandBuffers(std::ostream& os, CommandBufferDumpOptions options,
                          bool dump_all_command_buffers) const;
  void DumpAllCommandBuffers(std::ostream& os,
                             CommandBufferDumpOptions options) const;
  void DumpIncompleteCommandBuffers(std::ostream& os,
                                    CommandBufferDumpOptions options) const;
  void DumpCommandBufferStateOnScreen(CommandBuffer* p_cmd,
                                      std::ostream& os) const;

  void SetCommandPool(VkCommandPool vk_command_pool,
                      CommandPoolPtr command_pool);
  CommandPool* GetCommandPool(VkCommandPool vk_command_pool);
  void AllocateCommandBuffers(VkCommandPool vk_command_pool,
                              const VkCommandBufferAllocateInfo* allocate_info,
                              VkCommandBuffer* command_buffers);
  void ValidateCommandPoolState(VkCommandPool vk_command_pool,
                                std::ostream& os);
  void ResetCommandPool(VkCommandPool vk_command_pool);
  void DeleteCommandPool(VkCommandPool vk_command_pool);

  void CreatePipeline(uint32_t createInfoCount,
                      const VkGraphicsPipelineCreateInfo* pCreateInfos,
                      VkPipeline* pPipelines);
  void CreatePipeline(uint32_t createInfoCount,
                      const VkComputePipelineCreateInfo* pCreateInfos,
                      VkPipeline* pPipelines);
  const Pipeline* FindPipeline(VkPipeline pipeline) const;
  void DumpShaderFromPipeline(VkPipeline pipeline) const;
  void DeletePipeline(VkPipeline pipeline);

  void CreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo,
                          VkShaderModule* pShaderModule,
                          int shader_module_load_options);
  const ShaderModule* FindShaderModule(VkShaderModule shader_module) const;
  void DeleteShaderModule(VkShaderModule shaderModule);

  void RegisterQueueFamilyIndex(VkQueue queue, uint32_t queueFamilyIndex);
  uint32_t GetQueueFamilyIndex(VkQueue queue);

  void RegisterHelperCommandPool(uint32_t queueFamilyIndex,
                                 VkCommandPool commandPool);
  VkCommandPool GetHelperCommandPool(uint32_t queueFamilyIndex);
  std::vector<VkCommandPool> ReturnAndEraseCommandPools();

  bool AllocateMarker(Marker* marker);
  void FreeMarker(const Marker marker);

  std::ostream& Print(std::ostream& stream) const;

 private:
  GfrContext* gfr_ = nullptr;
  InstanceDispatchTable instance_dispatch_table_;
  DeviceDispatchTable device_dispatch_table_;
  VkPhysicalDevice vk_physical_device_ = VK_NULL_HANDLE;
  VkDevice vk_device_ = VK_NULL_HANDLE;
  bool has_buffer_marker_ = false;
  std::vector<VkQueueFamilyProperties> queue_family_properties_;
  VkPhysicalDeviceMemoryProperties memory_properties_ = {};
  VkPhysicalDeviceProperties physical_device_properties_ = {};

  SubmitTrackerPtr submit_tracker_;
  SemaphoreTrackerPtr semaphore_tracker_;

  std::unique_ptr<DeviceCreateInfo> device_create_info_;

  ObjectInfoDB object_info_db_;

  mutable std::recursive_mutex command_buffers_mutex_;
  std::vector<VkCommandBuffer> command_buffers_;

  std::mutex command_pools_mutex_;
  std::unordered_map<VkCommandPool, CommandPoolPtr> command_pools_;

  mutable std::mutex pipelines_mutex_;
  std::unordered_map<VkPipeline, PipelinePtr> pipelines_;

  mutable std::mutex shader_modules_mutex_;
  std::unordered_map<VkShaderModule, ShaderModulePtr> shader_modules_;

  // Tracks the queue index family used when creating queues. We need this info
  // to use a proper command pool when we create helper command buffers to
  // track the state of submits and semaphores.
  mutable std::mutex queue_family_index_trackers_mutex_;
  std::unordered_map<VkQueue, uint32_t /* queueFamilyIndex */>
      queue_family_index_trackers_;

  mutable std::mutex helper_command_pools_mutex_;
  std::unordered_map<uint32_t /* queueFamilyIndex */, VkCommandPool>
      helper_command_pools_;

  struct MarkerBuffer {
    VkDeviceSize size = 0;
    VkBuffer buffer = VK_NULL_HANDLE;
    void* cpu_mapped_address = nullptr;
    VkDeviceSize heap_offset = 0;
  };

  VkDeviceMemory marker_buffers_heap_ = VK_NULL_HANDLE;
  void* marker_buffers_heap_mapped_base_ = nullptr;
  VkDeviceSize current_heap_offset_ = 0;

  std::mutex marker_buffers_mutex_;
  std::vector<MarkerBuffer> marker_buffers_;
  uint32_t current_marker_index_ = 0;

  std::mutex recycled_markers_u32_mutex_;
  std::vector<Marker> recycled_markers_u32_;

  std::mutex recycled_markers_u64_mutex_;
  std::vector<Marker> recycled_markers_u64_;

  PFN_vkCmdWriteBufferMarkerAMD pfn_vkCmdWriteBufferMarkerAMD_ = nullptr;
  PFN_vkFreeCommandBuffers pfn_vkFreeCommandBuffers_ = nullptr;
};

using DevicePtr = std::unique_ptr<Device>;

}  // namespace GFR

#endif  // GFR_DEVICE_H
