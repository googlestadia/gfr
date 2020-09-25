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

#include "command_pool.h"

#include <algorithm>
#include <cassert>

namespace gfr {

CommandPool::CommandPool(
    VkCommandPool vk_command_pool, const VkCommandPoolCreateInfo* p_create_info,
    const std::vector<VkQueueFamilyProperties>& queue_family_properties,
    bool has_buffer_markers)
    : vk_command_pool_(vk_command_pool),
      has_buffer_markers_(has_buffer_markers),
      m_flags(p_create_info->flags) {}

void CommandPool::AllocateCommandBuffers(
    const VkCommandBufferAllocateInfo* allocate_info,
    const VkCommandBuffer* p_command_buffers) {
  if (allocate_info->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
    primary_command_buffers_.insert(
        primary_command_buffers_.end(), p_command_buffers,
        p_command_buffers + allocate_info->commandBufferCount);
  } else {
    secondary_command_buffers_.insert(
        secondary_command_buffers_.end(), p_command_buffers,
        p_command_buffers + allocate_info->commandBufferCount);
  }
}

void CommandPool::FreeCommandBuffers(uint32_t command_buffer_count,
                                     const VkCommandBuffer* p_command_buffers) {
  for (uint32_t i = 0; i < command_buffer_count; ++i) {
    std::vector<VkCommandBuffer>::iterator it = std::find(primary_command_buffers_.begin(),
                                                          primary_command_buffers_.end(),
                                                          p_command_buffers[i]);
    if (it != primary_command_buffers_.end()) {
      primary_command_buffers_.erase(it);
    }
    it = std::find(secondary_command_buffers_.begin(),
                   secondary_command_buffers_.end(), p_command_buffers[i]);
    if (it != secondary_command_buffers_.end()) {
      secondary_command_buffers_.erase(it);
    }
  }
}

const std::vector<VkCommandBuffer>& CommandPool::GetCommandBuffers(
    VkCommandBufferLevel level) const {
  if (level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
    return primary_command_buffers_;
  }
  return secondary_command_buffers_;
}

void CommandPool::Reset() {
  // reset all CBs
  // TODO CB Reset/ResetByPool tracking b/113674089
}

}  // namespace gfr
