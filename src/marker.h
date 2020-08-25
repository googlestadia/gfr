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

#ifndef GFR_MARKER_H
#define GFR_MARKER_H

#include <vulkan/vulkan.h>

enum MarkerType {
  kUint32,
  kUint64,
};

struct Marker {
  MarkerType type;
  VkBuffer buffer = VK_NULL_HANDLE;
  uint32_t offset = 0;
  void* cpu_mapped_address = nullptr;
};

#endif  // GFR_MARKER_H
