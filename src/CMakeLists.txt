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
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message("Please switch to x64 build.")
	return()
endif()
set(target_name gfr)

add_library(${target_name} MODULE
    ${CMAKE_CURRENT_SOURCE_DIR}/bind_sparse_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bind_sparse_utils.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/command.h
    ${CMAKE_CURRENT_SOURCE_DIR}/command.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/command_pool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/command_pool.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor_set.h
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor_set.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/device.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/gfr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gfr.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/linear_allocator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/marker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/object_name_db.h
    ${CMAKE_CURRENT_SOURCE_DIR}/object_name_db.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/semaphore_tracker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/semaphore_tracker.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_module.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_module.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_parse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/submit_tracker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/submit_tracker.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util.cc
    ${GENERATED_HEADER_FILES}
    ${GENERATED_SOURCE_FILES}
)

message(STATUS "Generated dir:${GENERATED_DIR}")
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                                  ${VULKAN_DIR}/include
                                                  ${GENERATED_DIR})

if(WIN32)
target_include_directories(${target_name} PRIVATE ${VULKAN_DIR}/LayerFactory/Project)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "VkLayer_gfr")
set_target_properties(${target_name} PROPERTIES CXX_VISIBILITY_PRESET hidden)

set_target_properties( ${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
