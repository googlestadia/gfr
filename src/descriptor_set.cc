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

#include "descriptor_set.h"

#include <sstream>

#include "gfr.h"
#include "util.h"

namespace GFR {

// =============================================================================
// ActiveDescriptorSets
// =============================================================================
void ActiveDescriptorSets::Reset() { descriptor_sets_.clear(); }

void ActiveDescriptorSets::Bind(uint32_t first_set, uint32_t set_count,
                                const VkDescriptorSet* sets) {
  for (uint32_t set_index = 0; set_index < set_count; ++set_index) {
    descriptor_sets_[set_index + first_set] = sets[set_index];
  }
}

std::ostream& ActiveDescriptorSets::Print(Device* device, std::ostream& stream,
                                          const std::string& indent) const {
  auto indent1 = GFR::IncreaseIndent(indent);
  auto indent2 = GFR::IncreaseIndent(indent1);
  for (const auto& ds : descriptor_sets_) {
    stream << indent1 << "- # descriptorSet:";
    stream << indent2 << "index: " << ds.first;
    stream << indent2 << "set: ";
    stream << device->GetObjectInfoNoHandleTag((uint64_t)ds.second, indent2);
  }

  return stream;
}

}  // namespace GFR
