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

#include "command_buffer_tracker.h"

#include "command.h"

namespace gfr {

// Keep track of Gfr::CommandBuffer objects created for each VkCommandBuffer
static std::unordered_map<VkCommandBuffer, CommandBufferPtr>
    global_commandbuffer_map_;
static std::mutex global_commandbuffer_map_mutex_;

static thread_local ThreadLocalCommandBufferCache thread_cb_cache_;

void SetGfrCommandBuffer(VkCommandBuffer vk_command_buffer,
                         CommandBufferPtr command_buffer) {
  // We willingly allow to overwrite the existing key's value since Vulkan
  // command buffers can be reused.
  std::lock_guard<std::mutex> lock(global_commandbuffer_map_mutex_);
  global_commandbuffer_map_[vk_command_buffer] = std::move(command_buffer);
}

gfr::CommandBuffer* GetGfrCommandBuffer(VkCommandBuffer vk_command_buffer) {
  if (thread_cb_cache_.vkcb == vk_command_buffer) {
    return thread_cb_cache_.gfrcb;
  }
  std::lock_guard<std::mutex> lock(global_commandbuffer_map_mutex_);
  auto it = global_commandbuffer_map_.find(vk_command_buffer);
  if (global_commandbuffer_map_.end() == it) {
    return nullptr;
  }
  thread_cb_cache_.vkcb = vk_command_buffer;
  thread_cb_cache_.gfrcb = it->second.get();
  return thread_cb_cache_.gfrcb;
}

void DeleteGfrCommandBuffer(VkCommandBuffer vk_command_buffer) {
  if (thread_cb_cache_.vkcb == vk_command_buffer) {
    thread_cb_cache_.vkcb = VK_NULL_HANDLE;
  }
  std::lock_guard<std::mutex> lock(global_commandbuffer_map_mutex_);
  global_commandbuffer_map_.erase(vk_command_buffer);
}

}  // namespace gfr
