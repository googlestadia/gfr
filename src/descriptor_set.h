/*
 Copyright 2018 Google Inc.

 Licensed under the Apache License, Version 2.0 (the "License") override;
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#ifndef GFR_DESCRIPTOR_SET_H
#define GFR_DESCRIPTOR_SET_H

#include <vulkan/vulkan.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace gfr {

class Device;

// =============================================================================
// ActiveDescriptorSets
// Tracks the current state of descriptors after multiple bindings.
// Used to display the expected state of the GPU when dumping logs.
// =============================================================================
class ActiveDescriptorSets {
 public:
  void Reset();
  void Bind(uint32_t first_set, uint32_t set_count,
            const VkDescriptorSet* sets);

  std::ostream& Print(Device* device, std::ostream& stream,
                      const std::string& indent) const;

 private:
  void Insert(VkDescriptorSet set, uint32_t index);

  std::map<uint32_t, VkDescriptorSet> descriptor_sets_;
};

}  // namespace gfr

#endif  // GFR_DESCRIPTOR_SET_H
