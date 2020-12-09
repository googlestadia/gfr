/*
 * Copyright (C) 2015 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * THIS FILE WAS GENERATED BY apic. DO NOT EDIT.
 */

// clang-format off
#ifndef COMMAND_COMMON_HEADER
#define COMMAND_COMMON_HEADER

#include <cstring>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

struct Command
{
  enum Type {
    kUnknown,
    kBeginCommandBuffer,
    kEndCommandBuffer,
    kResetCommandBuffer,
    kExecuteCommands,
    kCopyBuffer,
    kCopyImage,
    kBlitImage,
    kCopyBufferToImage,
    kCopyImageToBuffer,
    kUpdateBuffer,
    kFillBuffer,
    kClearColorImage,
    kClearDepthStencilImage,
    kClearAttachments,
    kResolveImage,
    kBindDescriptorSets,
    kPushConstants,
    kBindIndexBuffer,
    kBindVertexBuffers,
    kDraw,
    kDrawIndexed,
    kDrawIndirect,
    kDrawIndexedIndirect,
    kDispatch,
    kDispatchIndirect,
    kBindPipeline,
    kSetViewport,
    kSetScissor,
    kSetLineWidth,
    kSetDepthBias,
    kSetBlendConstants,
    kSetDepthBounds,
    kSetStencilCompareMask,
    kSetStencilWriteMask,
    kSetStencilReference,
    kBeginQuery,
    kEndQuery,
    kResetQueryPool,
    kWriteTimestamp,
    kCopyQueryPoolResults,
    kBeginRenderPass,
    kNextSubpass,
    kEndRenderPass,
    kSetEvent,
    kResetEvent,
    kWaitEvents,
    kPipelineBarrier,
    kWriteBufferMarkerAMD,
    kDrawIndirectCountAMD,
    kDrawIndexedIndirectCountAMD,
    kBeginConditionalRenderingEXT,
    kEndConditionalRenderingEXT,
    kDebugMarkerBeginEXT,
    kDebugMarkerEndEXT,
    kDebugMarkerInsertEXT,
    kBeginDebugUtilsLabelEXT,
    kEndDebugUtilsLabelEXT,
    kInsertDebugUtilsLabelEXT,
    kSetDeviceMaskKHR,
    kSetDeviceMask,
    kDispatchBaseKHR,
    kDispatchBase,
    kDrawIndirectCountKHR,
    kDrawIndexedIndirectCountKHR,
  };

  public:
  static const char *GetCommandName(const Command &cmd);

  Type type;
  uint32_t id;
  void *parameters;
};
#endif // COMMAND_COMMON_HEADER