/*
 Copyright 2019 Google Inc.

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

#include "object_name_db.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "util.h"

namespace {

template <typename T>
std::string PtrToStr(const T* ptr) {
  uintptr_t value = (uintptr_t)ptr;
  return gfr::Uint64ToStr(value);
}
}  // namespace

ObjectInfoDB::ObjectInfoDB() {
  // default name for unknown objects
  unknown_object_.object = 0;
  unknown_object_.type = VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
  unknown_object_.name = "<unknown>";
}

void ObjectInfoDB::AddObjectInfo(uint64_t handle, ObjectInfoPtr info) {
  std::lock_guard<std::mutex> lock(lock_);
  object_info_[handle] = std::move(info);
}

void ObjectInfoDB::AddExtraInfo(uint64_t handle, ExtraObjectInfo info) {
  std::lock_guard<std::mutex> lock(lock_);
  object_extra_info_[handle].push_back(info);
}

const ObjectInfo* ObjectInfoDB::FindObjectInfo(uint64_t handle) const {
  std::lock_guard<std::mutex> lock(lock_);

  auto handle_info = object_info_.find(handle);
  if (end(object_info_) != handle_info) {
    return handle_info->second.get();
  }

  return &unknown_object_;
}

std::string ObjectInfoDB::GetObjectName(
    uint64_t handle,
    HandleDebugNamePreference handle_debug_name_preference) const {
  auto info = FindObjectInfo(handle);
  if (handle_debug_name_preference == kPreferDebugName) {
    if (info != &unknown_object_) {
      return info->name;
    }
    return gfr::Uint64ToStr(handle);
  }
  std::stringstream object_name;
  if (info != &unknown_object_) {
    object_name << info->name << " ";
  }
  object_name << "(" << gfr::Uint64ToStr(handle) << ")";
  return object_name.str();
}

std::string ObjectInfoDB::GetObjectInfoInternal(
    uint64_t handle, const std::string& indent,
    VkHandleTagRequirement vkhandle_tag_requirement) const {
  // TODO(aellem) cleanup so all object are tracked and debug object names only
  // enhance object names
  std::stringstream info_ss;
  if (vkhandle_tag_requirement == kPrintVkHandleTag) {
    info_ss << indent << "vkHandle: ";
  }
  info_ss << gfr::Uint64ToStr(handle);
  auto info = FindObjectInfo(handle);
  if (info != &unknown_object_) {
    info_ss << indent << "debugName: \"" << info->name << "\"";
  }
  std::lock_guard<std::mutex> lock(lock_);
  auto extra_infos = object_extra_info_.find(handle);
  if (end(object_extra_info_) != extra_infos) {
    for (auto& extra_info : extra_infos->second) {
      info_ss << indent << extra_info.first << ": " << extra_info.second;
    }
  }

  return info_ss.str();
}

std::string ObjectInfoDB::GetObjectInfo(uint64_t handle,
                                        const std::string& indent) const {
  return GetObjectInfoInternal(handle, indent, kPrintVkHandleTag);
}

std::string ObjectInfoDB::GetObjectInfoNoHandleTag(
    uint64_t handle, const std::string& indent) const {
  return GetObjectInfoInternal(handle, indent, kIgnoreVkHandleTag);
}
