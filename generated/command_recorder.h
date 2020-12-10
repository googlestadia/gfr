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
#ifndef COMMAND_RECORDER_HEADER
#define COMMAND_RECORDER_HEADER

#include <cstring>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

#include "linear_allocator.h"
#include "object_name_db.h"

struct BeginCommandBufferArgs {
  VkCommandBuffer commandBuffer;
  VkCommandBufferBeginInfo const* pBeginInfo;
};

struct EndCommandBufferArgs {
  VkCommandBuffer commandBuffer;
};

struct ResetCommandBufferArgs {
  VkCommandBuffer commandBuffer;
  VkCommandBufferResetFlags flags;
};

struct CmdExecuteCommandsArgs {
  VkCommandBuffer commandBuffer;
  uint32_t commandBufferCount;
  VkCommandBuffer const* pCommandBuffers;
};

struct CmdCopyBufferArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer srcBuffer;
  VkBuffer dstBuffer;
  uint32_t regionCount;
  VkBufferCopy const* pRegions;
};

struct CmdCopyImageArgs {
  VkCommandBuffer commandBuffer;
  VkImage srcImage;
  VkImageLayout srcImageLayout;
  VkImage dstImage;
  VkImageLayout dstImageLayout;
  uint32_t regionCount;
  VkImageCopy const* pRegions;
};

struct CmdBlitImageArgs {
  VkCommandBuffer commandBuffer;
  VkImage srcImage;
  VkImageLayout srcImageLayout;
  VkImage dstImage;
  VkImageLayout dstImageLayout;
  uint32_t regionCount;
  VkImageBlit const* pRegions;
  VkFilter filter;
};

struct CmdCopyBufferToImageArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer srcBuffer;
  VkImage dstImage;
  VkImageLayout dstImageLayout;
  uint32_t regionCount;
  VkBufferImageCopy const* pRegions;
};

struct CmdCopyImageToBufferArgs {
  VkCommandBuffer commandBuffer;
  VkImage srcImage;
  VkImageLayout srcImageLayout;
  VkBuffer dstBuffer;
  uint32_t regionCount;
  VkBufferImageCopy const* pRegions;
};

struct CmdUpdateBufferArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer dstBuffer;
  VkDeviceSize dstOffset;
  VkDeviceSize dataSize;
  void const* pData;
};

struct CmdFillBufferArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer dstBuffer;
  VkDeviceSize dstOffset;
  VkDeviceSize size;
  uint32_t data;
};

struct CmdClearColorImageArgs {
  VkCommandBuffer commandBuffer;
  VkImage image;
  VkImageLayout imageLayout;
  VkClearColorValue const* pColor;
  uint32_t rangeCount;
  VkImageSubresourceRange const* pRanges;
};

struct CmdClearDepthStencilImageArgs {
  VkCommandBuffer commandBuffer;
  VkImage image;
  VkImageLayout imageLayout;
  VkClearDepthStencilValue const* pDepthStencil;
  uint32_t rangeCount;
  VkImageSubresourceRange const* pRanges;
};

struct CmdClearAttachmentsArgs {
  VkCommandBuffer commandBuffer;
  uint32_t attachmentCount;
  VkClearAttachment const* pAttachments;
  uint32_t rectCount;
  VkClearRect const* pRects;
};

struct CmdResolveImageArgs {
  VkCommandBuffer commandBuffer;
  VkImage srcImage;
  VkImageLayout srcImageLayout;
  VkImage dstImage;
  VkImageLayout dstImageLayout;
  uint32_t regionCount;
  VkImageResolve const* pRegions;
};

struct CmdBindDescriptorSetsArgs {
  VkCommandBuffer commandBuffer;
  VkPipelineBindPoint pipelineBindPoint;
  VkPipelineLayout layout;
  uint32_t firstSet;
  uint32_t descriptorSetCount;
  VkDescriptorSet const* pDescriptorSets;
  uint32_t dynamicOffsetCount;
  uint32_t const* pDynamicOffsets;
};

struct CmdPushConstantsArgs {
  VkCommandBuffer commandBuffer;
  VkPipelineLayout layout;
  VkShaderStageFlags stageFlags;
  uint32_t offset;
  uint32_t size;
  void const* pValues;
};

struct CmdBindIndexBufferArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  VkIndexType indexType;
};

struct CmdBindVertexBuffersArgs {
  VkCommandBuffer commandBuffer;
  uint32_t firstBinding;
  uint32_t bindingCount;
  VkBuffer const* pBuffers;
  VkDeviceSize const* pOffsets;
};

struct CmdDrawArgs {
  VkCommandBuffer commandBuffer;
  uint32_t vertexCount;
  uint32_t instanceCount;
  uint32_t firstVertex;
  uint32_t firstInstance;
};

struct CmdDrawIndexedArgs {
  VkCommandBuffer commandBuffer;
  uint32_t indexCount;
  uint32_t instanceCount;
  uint32_t firstIndex;
  int32_t vertexOffset;
  uint32_t firstInstance;
};

struct CmdDrawIndirectArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  uint32_t drawCount;
  uint32_t stride;
};

struct CmdDrawIndexedIndirectArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  uint32_t drawCount;
  uint32_t stride;
};

struct CmdDispatchArgs {
  VkCommandBuffer commandBuffer;
  uint32_t groupCountX;
  uint32_t groupCountY;
  uint32_t groupCountZ;
};

struct CmdDispatchIndirectArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
};

struct CmdBindPipelineArgs {
  VkCommandBuffer commandBuffer;
  VkPipelineBindPoint pipelineBindPoint;
  VkPipeline pipeline;
};

struct CmdSetViewportArgs {
  VkCommandBuffer commandBuffer;
  uint32_t firstViewport;
  uint32_t viewportCount;
  VkViewport const* pViewports;
};

struct CmdSetScissorArgs {
  VkCommandBuffer commandBuffer;
  uint32_t firstScissor;
  uint32_t scissorCount;
  VkRect2D const* pScissors;
};

struct CmdSetLineWidthArgs {
  VkCommandBuffer commandBuffer;
  float lineWidth;
};

struct CmdSetDepthBiasArgs {
  VkCommandBuffer commandBuffer;
  float depthBiasConstantFactor;
  float depthBiasClamp;
  float depthBiasSlopeFactor;
};

struct CmdSetBlendConstantsArgs {
  VkCommandBuffer commandBuffer;
  float blendConstants[4];
};

struct CmdSetDepthBoundsArgs {
  VkCommandBuffer commandBuffer;
  float minDepthBounds;
  float maxDepthBounds;
};

struct CmdSetStencilCompareMaskArgs {
  VkCommandBuffer commandBuffer;
  VkStencilFaceFlags faceMask;
  uint32_t compareMask;
};

struct CmdSetStencilWriteMaskArgs {
  VkCommandBuffer commandBuffer;
  VkStencilFaceFlags faceMask;
  uint32_t writeMask;
};

struct CmdSetStencilReferenceArgs {
  VkCommandBuffer commandBuffer;
  VkStencilFaceFlags faceMask;
  uint32_t reference;
};

struct CmdBeginQueryArgs {
  VkCommandBuffer commandBuffer;
  VkQueryPool queryPool;
  uint32_t query;
  VkQueryControlFlags flags;
};

struct CmdEndQueryArgs {
  VkCommandBuffer commandBuffer;
  VkQueryPool queryPool;
  uint32_t query;
};

struct CmdResetQueryPoolArgs {
  VkCommandBuffer commandBuffer;
  VkQueryPool queryPool;
  uint32_t firstQuery;
  uint32_t queryCount;
};

struct CmdWriteTimestampArgs {
  VkCommandBuffer commandBuffer;
  VkPipelineStageFlagBits pipelineStage;
  VkQueryPool queryPool;
  uint32_t query;
};

struct CmdCopyQueryPoolResultsArgs {
  VkCommandBuffer commandBuffer;
  VkQueryPool queryPool;
  uint32_t firstQuery;
  uint32_t queryCount;
  VkBuffer dstBuffer;
  VkDeviceSize dstOffset;
  VkDeviceSize stride;
  VkQueryResultFlags flags;
};

struct CmdBeginRenderPassArgs {
  VkCommandBuffer commandBuffer;
  VkRenderPassBeginInfo const* pRenderPassBegin;
  VkSubpassContents contents;
};

struct CmdNextSubpassArgs {
  VkCommandBuffer commandBuffer;
  VkSubpassContents contents;
};

struct CmdEndRenderPassArgs {
  VkCommandBuffer commandBuffer;
};

struct CmdSetEventArgs {
  VkCommandBuffer commandBuffer;
  VkEvent event;
  VkPipelineStageFlags stageMask;
};

struct CmdResetEventArgs {
  VkCommandBuffer commandBuffer;
  VkEvent event;
  VkPipelineStageFlags stageMask;
};

struct CmdWaitEventsArgs {
  VkCommandBuffer commandBuffer;
  uint32_t eventCount;
  VkEvent const* pEvents;
  VkPipelineStageFlags srcStageMask;
  VkPipelineStageFlags dstStageMask;
  uint32_t memoryBarrierCount;
  VkMemoryBarrier const* pMemoryBarriers;
  uint32_t bufferMemoryBarrierCount;
  VkBufferMemoryBarrier const* pBufferMemoryBarriers;
  uint32_t imageMemoryBarrierCount;
  VkImageMemoryBarrier const* pImageMemoryBarriers;
};

struct CmdPipelineBarrierArgs {
  VkCommandBuffer commandBuffer;
  VkPipelineStageFlags srcStageMask;
  VkPipelineStageFlags dstStageMask;
  VkDependencyFlags dependencyFlags;
  uint32_t memoryBarrierCount;
  VkMemoryBarrier const* pMemoryBarriers;
  uint32_t bufferMemoryBarrierCount;
  VkBufferMemoryBarrier const* pBufferMemoryBarriers;
  uint32_t imageMemoryBarrierCount;
  VkImageMemoryBarrier const* pImageMemoryBarriers;
};

struct CmdWriteBufferMarkerAMDArgs {
  VkCommandBuffer commandBuffer;
  VkPipelineStageFlagBits pipelineStage;
  VkBuffer dstBuffer;
  VkDeviceSize dstOffset;
  uint32_t marker;
};

struct CmdDrawIndirectCountAMDArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  VkBuffer countBuffer;
  VkDeviceSize countOffset;
  uint32_t maxDrawCount;
  uint32_t stride;
};

struct CmdDrawIndexedIndirectCountAMDArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  VkBuffer countBuffer;
  VkDeviceSize countOffset;
  uint32_t maxDrawCount;
  uint32_t stride;
};

struct CmdBeginConditionalRenderingEXTArgs {
  VkCommandBuffer commandBuffer;
  VkConditionalRenderingBeginInfoEXT const* pConditinalRenderingBegin;
};

struct CmdEndConditionalRenderingEXTArgs {
  VkCommandBuffer commandBuffer;
};

struct CmdDebugMarkerBeginEXTArgs {
  VkCommandBuffer commandBuffer;
  VkDebugMarkerMarkerInfoEXT const* pMarkerInfo;
};

struct CmdDebugMarkerEndEXTArgs {
  VkCommandBuffer commandBuffer;
};

struct CmdDebugMarkerInsertEXTArgs {
  VkCommandBuffer commandBuffer;
  VkDebugMarkerMarkerInfoEXT const* pMarkerInfo;
};

struct CmdBeginDebugUtilsLabelEXTArgs {
  VkCommandBuffer commandBuffer;
  VkDebugUtilsLabelEXT const* pLabelInfo;
};

struct CmdEndDebugUtilsLabelEXTArgs {
  VkCommandBuffer commandBuffer;
};

struct CmdInsertDebugUtilsLabelEXTArgs {
  VkCommandBuffer commandBuffer;
  VkDebugUtilsLabelEXT const* pLabelInfo;
};

struct CmdSetDeviceMaskKHRArgs {
  VkCommandBuffer commandBuffer;
  uint32_t deviceMask;
};

struct CmdSetDeviceMaskArgs {
  VkCommandBuffer commandBuffer;
  uint32_t deviceMask;
};

struct CmdDispatchBaseKHRArgs {
  VkCommandBuffer commandBuffer;
  uint32_t baseGroupX;
  uint32_t baseGroupY;
  uint32_t baseGroupZ;
  uint32_t groupCountX;
  uint32_t groupCountY;
  uint32_t groupCountZ;
};

struct CmdDispatchBaseArgs {
  VkCommandBuffer commandBuffer;
  uint32_t baseGroupX;
  uint32_t baseGroupY;
  uint32_t baseGroupZ;
  uint32_t groupCountX;
  uint32_t groupCountY;
  uint32_t groupCountZ;
};

struct CmdDrawIndirectCountKHRArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  VkBuffer countBuffer;
  VkDeviceSize countOffset;
  uint32_t maxDrawCount;
  uint32_t stride;
};

struct CmdDrawIndexedIndirectCountKHRArgs {
  VkCommandBuffer commandBuffer;
  VkBuffer buffer;
  VkDeviceSize offset;
  VkBuffer countBuffer;
  VkDeviceSize countOffset;
  uint32_t maxDrawCount;
  uint32_t stride;
};


class CommandRecorder
{
  public:
  void Reset() { m_allocator.Reset(); }
  void SetNameResolver(std::ostream &os, const ObjectInfoDB *name_resolver);

  void PrintBeginCommandBufferArgs(std::ostream &os, const BeginCommandBufferArgs &args, const std::string& indent);
  BeginCommandBufferArgs *RecordBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo const* pBeginInfo);
  void PrintEndCommandBufferArgs(std::ostream &os, const EndCommandBufferArgs &args, const std::string& indent);
  EndCommandBufferArgs *RecordEndCommandBuffer(VkCommandBuffer commandBuffer);
  void PrintResetCommandBufferArgs(std::ostream &os, const ResetCommandBufferArgs &args, const std::string& indent);
  ResetCommandBufferArgs *RecordResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
  void PrintCmdExecuteCommandsArgs(std::ostream &os, const CmdExecuteCommandsArgs &args, const std::string& indent);
  CmdExecuteCommandsArgs *RecordCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer const* pCommandBuffers);
  void PrintCmdCopyBufferArgs(std::ostream &os, const CmdCopyBufferArgs &args, const std::string& indent);
  CmdCopyBufferArgs *RecordCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy const* pRegions);
  void PrintCmdCopyImageArgs(std::ostream &os, const CmdCopyImageArgs &args, const std::string& indent);
  CmdCopyImageArgs *RecordCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy const* pRegions);
  void PrintCmdBlitImageArgs(std::ostream &os, const CmdBlitImageArgs &args, const std::string& indent);
  CmdBlitImageArgs *RecordCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit const* pRegions, VkFilter filter);
  void PrintCmdCopyBufferToImageArgs(std::ostream &os, const CmdCopyBufferToImageArgs &args, const std::string& indent);
  CmdCopyBufferToImageArgs *RecordCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy const* pRegions);
  void PrintCmdCopyImageToBufferArgs(std::ostream &os, const CmdCopyImageToBufferArgs &args, const std::string& indent);
  CmdCopyImageToBufferArgs *RecordCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy const* pRegions);
  void PrintCmdUpdateBufferArgs(std::ostream &os, const CmdUpdateBufferArgs &args, const std::string& indent);
  CmdUpdateBufferArgs *RecordCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void const* pData);
  void PrintCmdFillBufferArgs(std::ostream &os, const CmdFillBufferArgs &args, const std::string& indent);
  CmdFillBufferArgs *RecordCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
  void PrintCmdClearColorImageArgs(std::ostream &os, const CmdClearColorImageArgs &args, const std::string& indent);
  CmdClearColorImageArgs *RecordCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue const* pColor, uint32_t rangeCount, VkImageSubresourceRange const* pRanges);
  void PrintCmdClearDepthStencilImageArgs(std::ostream &os, const CmdClearDepthStencilImageArgs &args, const std::string& indent);
  CmdClearDepthStencilImageArgs *RecordCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue const* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange const* pRanges);
  void PrintCmdClearAttachmentsArgs(std::ostream &os, const CmdClearAttachmentsArgs &args, const std::string& indent);
  CmdClearAttachmentsArgs *RecordCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment const* pAttachments, uint32_t rectCount, VkClearRect const* pRects);
  void PrintCmdResolveImageArgs(std::ostream &os, const CmdResolveImageArgs &args, const std::string& indent);
  CmdResolveImageArgs *RecordCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve const* pRegions);
  void PrintCmdBindDescriptorSetsArgs(std::ostream &os, const CmdBindDescriptorSetsArgs &args, const std::string& indent);
  CmdBindDescriptorSetsArgs *RecordCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet const* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t const* pDynamicOffsets);
  void PrintCmdPushConstantsArgs(std::ostream &os, const CmdPushConstantsArgs &args, const std::string& indent);
  CmdPushConstantsArgs *RecordCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void const* pValues);
  void PrintCmdBindIndexBufferArgs(std::ostream &os, const CmdBindIndexBufferArgs &args, const std::string& indent);
  CmdBindIndexBufferArgs *RecordCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
  void PrintCmdBindVertexBuffersArgs(std::ostream &os, const CmdBindVertexBuffersArgs &args, const std::string& indent);
  CmdBindVertexBuffersArgs *RecordCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer const* pBuffers, VkDeviceSize const* pOffsets);
  void PrintCmdDrawArgs(std::ostream &os, const CmdDrawArgs &args, const std::string& indent);
  CmdDrawArgs *RecordCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
  void PrintCmdDrawIndexedArgs(std::ostream &os, const CmdDrawIndexedArgs &args, const std::string& indent);
  CmdDrawIndexedArgs *RecordCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
  void PrintCmdDrawIndirectArgs(std::ostream &os, const CmdDrawIndirectArgs &args, const std::string& indent);
  CmdDrawIndirectArgs *RecordCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
  void PrintCmdDrawIndexedIndirectArgs(std::ostream &os, const CmdDrawIndexedIndirectArgs &args, const std::string& indent);
  CmdDrawIndexedIndirectArgs *RecordCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
  void PrintCmdDispatchArgs(std::ostream &os, const CmdDispatchArgs &args, const std::string& indent);
  CmdDispatchArgs *RecordCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
  void PrintCmdDispatchIndirectArgs(std::ostream &os, const CmdDispatchIndirectArgs &args, const std::string& indent);
  CmdDispatchIndirectArgs *RecordCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
  void PrintCmdBindPipelineArgs(std::ostream &os, const CmdBindPipelineArgs &args, const std::string& indent);
  CmdBindPipelineArgs *RecordCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
  void PrintCmdSetViewportArgs(std::ostream &os, const CmdSetViewportArgs &args, const std::string& indent);
  CmdSetViewportArgs *RecordCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport const* pViewports);
  void PrintCmdSetScissorArgs(std::ostream &os, const CmdSetScissorArgs &args, const std::string& indent);
  CmdSetScissorArgs *RecordCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D const* pScissors);
  void PrintCmdSetLineWidthArgs(std::ostream &os, const CmdSetLineWidthArgs &args, const std::string& indent);
  CmdSetLineWidthArgs *RecordCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth);
  void PrintCmdSetDepthBiasArgs(std::ostream &os, const CmdSetDepthBiasArgs &args, const std::string& indent);
  CmdSetDepthBiasArgs *RecordCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
  void PrintCmdSetBlendConstantsArgs(std::ostream &os, const CmdSetBlendConstantsArgs &args, const std::string& indent);
  CmdSetBlendConstantsArgs *RecordCmdSetBlendConstants(VkCommandBuffer commandBuffer, float blendConstants[4]);
  void PrintCmdSetDepthBoundsArgs(std::ostream &os, const CmdSetDepthBoundsArgs &args, const std::string& indent);
  CmdSetDepthBoundsArgs *RecordCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
  void PrintCmdSetStencilCompareMaskArgs(std::ostream &os, const CmdSetStencilCompareMaskArgs &args, const std::string& indent);
  CmdSetStencilCompareMaskArgs *RecordCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
  void PrintCmdSetStencilWriteMaskArgs(std::ostream &os, const CmdSetStencilWriteMaskArgs &args, const std::string& indent);
  CmdSetStencilWriteMaskArgs *RecordCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
  void PrintCmdSetStencilReferenceArgs(std::ostream &os, const CmdSetStencilReferenceArgs &args, const std::string& indent);
  CmdSetStencilReferenceArgs *RecordCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
  void PrintCmdBeginQueryArgs(std::ostream &os, const CmdBeginQueryArgs &args, const std::string& indent);
  CmdBeginQueryArgs *RecordCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
  void PrintCmdEndQueryArgs(std::ostream &os, const CmdEndQueryArgs &args, const std::string& indent);
  CmdEndQueryArgs *RecordCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
  void PrintCmdResetQueryPoolArgs(std::ostream &os, const CmdResetQueryPoolArgs &args, const std::string& indent);
  CmdResetQueryPoolArgs *RecordCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
  void PrintCmdWriteTimestampArgs(std::ostream &os, const CmdWriteTimestampArgs &args, const std::string& indent);
  CmdWriteTimestampArgs *RecordCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
  void PrintCmdCopyQueryPoolResultsArgs(std::ostream &os, const CmdCopyQueryPoolResultsArgs &args, const std::string& indent);
  CmdCopyQueryPoolResultsArgs *RecordCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
  void PrintCmdBeginRenderPassArgs(std::ostream &os, const CmdBeginRenderPassArgs &args, const std::string& indent);
  CmdBeginRenderPassArgs *RecordCmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo const* pRenderPassBegin, VkSubpassContents contents);
  void PrintCmdNextSubpassArgs(std::ostream &os, const CmdNextSubpassArgs &args, const std::string& indent);
  CmdNextSubpassArgs *RecordCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents);
  void PrintCmdEndRenderPassArgs(std::ostream &os, const CmdEndRenderPassArgs &args, const std::string& indent);
  CmdEndRenderPassArgs *RecordCmdEndRenderPass(VkCommandBuffer commandBuffer);
  void PrintCmdSetEventArgs(std::ostream &os, const CmdSetEventArgs &args, const std::string& indent);
  CmdSetEventArgs *RecordCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
  void PrintCmdResetEventArgs(std::ostream &os, const CmdResetEventArgs &args, const std::string& indent);
  CmdResetEventArgs *RecordCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
  void PrintCmdWaitEventsArgs(std::ostream &os, const CmdWaitEventsArgs &args, const std::string& indent);
  CmdWaitEventsArgs *RecordCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent const* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier const* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier const* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier const* pImageMemoryBarriers);
  void PrintCmdPipelineBarrierArgs(std::ostream &os, const CmdPipelineBarrierArgs &args, const std::string& indent);
  CmdPipelineBarrierArgs *RecordCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier const* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier const* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier const* pImageMemoryBarriers);
  void PrintCmdWriteBufferMarkerAMDArgs(std::ostream &os, const CmdWriteBufferMarkerAMDArgs &args, const std::string& indent);
  CmdWriteBufferMarkerAMDArgs *RecordCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
  void PrintCmdDrawIndirectCountAMDArgs(std::ostream &os, const CmdDrawIndirectCountAMDArgs &args, const std::string& indent);
  CmdDrawIndirectCountAMDArgs *RecordCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countOffset, uint32_t maxDrawCount, uint32_t stride);
  void PrintCmdDrawIndexedIndirectCountAMDArgs(std::ostream &os, const CmdDrawIndexedIndirectCountAMDArgs &args, const std::string& indent);
  CmdDrawIndexedIndirectCountAMDArgs *RecordCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countOffset, uint32_t maxDrawCount, uint32_t stride);
  void PrintCmdBeginConditionalRenderingEXTArgs(std::ostream &os, const CmdBeginConditionalRenderingEXTArgs &args, const std::string& indent);
  CmdBeginConditionalRenderingEXTArgs *RecordCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, VkConditionalRenderingBeginInfoEXT const* pConditinalRenderingBegin);
  void PrintCmdEndConditionalRenderingEXTArgs(std::ostream &os, const CmdEndConditionalRenderingEXTArgs &args, const std::string& indent);
  CmdEndConditionalRenderingEXTArgs *RecordCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer);
  void PrintCmdDebugMarkerBeginEXTArgs(std::ostream &os, const CmdDebugMarkerBeginEXTArgs &args, const std::string& indent);
  CmdDebugMarkerBeginEXTArgs *RecordCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT const* pMarkerInfo);
  void PrintCmdDebugMarkerEndEXTArgs(std::ostream &os, const CmdDebugMarkerEndEXTArgs &args, const std::string& indent);
  CmdDebugMarkerEndEXTArgs *RecordCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer);
  void PrintCmdDebugMarkerInsertEXTArgs(std::ostream &os, const CmdDebugMarkerInsertEXTArgs &args, const std::string& indent);
  CmdDebugMarkerInsertEXTArgs *RecordCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT const* pMarkerInfo);
  void PrintCmdBeginDebugUtilsLabelEXTArgs(std::ostream &os, const CmdBeginDebugUtilsLabelEXTArgs &args, const std::string& indent);
  CmdBeginDebugUtilsLabelEXTArgs *RecordCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT const* pLabelInfo);
  void PrintCmdEndDebugUtilsLabelEXTArgs(std::ostream &os, const CmdEndDebugUtilsLabelEXTArgs &args, const std::string& indent);
  CmdEndDebugUtilsLabelEXTArgs *RecordCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer);
  void PrintCmdInsertDebugUtilsLabelEXTArgs(std::ostream &os, const CmdInsertDebugUtilsLabelEXTArgs &args, const std::string& indent);
  CmdInsertDebugUtilsLabelEXTArgs *RecordCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT const* pLabelInfo);
  void PrintCmdSetDeviceMaskKHRArgs(std::ostream &os, const CmdSetDeviceMaskKHRArgs &args, const std::string& indent);
  CmdSetDeviceMaskKHRArgs *RecordCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask);
  void PrintCmdSetDeviceMaskArgs(std::ostream &os, const CmdSetDeviceMaskArgs &args, const std::string& indent);
  CmdSetDeviceMaskArgs *RecordCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask);
  void PrintCmdDispatchBaseKHRArgs(std::ostream &os, const CmdDispatchBaseKHRArgs &args, const std::string& indent);
  CmdDispatchBaseKHRArgs *RecordCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
  void PrintCmdDispatchBaseArgs(std::ostream &os, const CmdDispatchBaseArgs &args, const std::string& indent);
  CmdDispatchBaseArgs *RecordCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
  void PrintCmdDrawIndirectCountKHRArgs(std::ostream &os, const CmdDrawIndirectCountKHRArgs &args, const std::string& indent);
  CmdDrawIndirectCountKHRArgs *RecordCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countOffset, uint32_t maxDrawCount, uint32_t stride);
  void PrintCmdDrawIndexedIndirectCountKHRArgs(std::ostream &os, const CmdDrawIndexedIndirectCountKHRArgs &args, const std::string& indent);
  CmdDrawIndexedIndirectCountKHRArgs *RecordCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countOffset, uint32_t maxDrawCount, uint32_t stride);

  private:
  template <typename T> T *Alloc() { return new(m_allocator.Alloc(sizeof(T))) T; }
  template <typename T> T *CopyArray(const T *src, uint64_t start_index, uint64_t count) {
    auto ptr = reinterpret_cast<T *>(m_allocator.Alloc(sizeof(T) * count));
    std::memcpy(ptr, src, sizeof(T) * count);
    return ptr;
  }

  LinearAllocator<> m_allocator;
};
#endif // COMMAND_RECORDER_HEADER