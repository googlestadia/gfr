# Copyright (C) 2020 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.0)
project(GFR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(VULKAN_DIR_FOUND FALSE)
if (NOT VULKAN_DIR)
  set(VULKAN_DIR $ENV{VULKAN_SDK})
endif()
if (NOT "${VULKAN_DIR}" STREQUAL "")
  message(STATUS "Using Vulkan found at ${VULKAN_DIR}")
  set(VULKAN_DIR_FOUND TRUE)
endif()
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
file(TO_CMAKE_PATH ${VULKAN_DIR} VULKAN_DIR)

set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/generated")
list(APPEND GENERATED_HEADER_FILES
  ${GENERATED_DIR}/command_common.h
  ${GENERATED_DIR}/command_printer.h
  ${GENERATED_DIR}/command_recorder.h
  ${GENERATED_DIR}/command_tracker.h
  ${GENERATED_DIR}/dispatch.h
  ${GENERATED_DIR}/layer_base.h
)
list(APPEND GENERATED_SOURCE_FILES
  ${GENERATED_DIR}/command_common.cc
  ${GENERATED_DIR}/command_printer.cc
  ${GENERATED_DIR}/command_recorder.cc
  ${GENERATED_DIR}/command_tracker.cc
  ${GENERATED_DIR}/dispatch.cc
  ${GENERATED_DIR}/layer_base.cc
)
message(STATUS "Generated dir:${GENERATED_DIR}")
message(STATUS "${GENERATED_SOURCE_FILES}")

add_subdirectory(src)
