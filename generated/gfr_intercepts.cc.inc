

// clang-format off



void InterceptPreCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) {
  g_interceptor->PreCmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}
void InterceptPostCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) {
  g_interceptor->PostCmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void InterceptPreCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) {
  g_interceptor->PreCmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}
void InterceptPostCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) {
  g_interceptor->PostCmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

VkResult InterceptPostAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex, VkResult result) {
  return g_interceptor->PostAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex, result);
}
VkResult InterceptPostQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo, VkResult result) {
  return g_interceptor->PostQueuePresentKHR(queue, pPresentInfo, result);
}
void InterceptPreDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) {
  g_interceptor->PreDebugMarkerSetObjectNameEXT(device, pNameInfo);
}
VkResult InterceptPostDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo, VkResult result) {
  return g_interceptor->PostDebugMarkerSetObjectNameEXT(device, pNameInfo, result);
}
void InterceptPreCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
  g_interceptor->PreCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}
void InterceptPostCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
  g_interceptor->PostCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}

void InterceptPreCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
  g_interceptor->PreCmdDebugMarkerEndEXT(commandBuffer);
}
void InterceptPostCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
  g_interceptor->PostCmdDebugMarkerEndEXT(commandBuffer);
}

void InterceptPreCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
  g_interceptor->PreCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}
void InterceptPostCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
  g_interceptor->PostCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}

void InterceptPreCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
  g_interceptor->PreCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
void InterceptPostCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
  g_interceptor->PostCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void InterceptPreCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
  g_interceptor->PreCmdSetDeviceMask(commandBuffer, deviceMask);
}
void InterceptPostCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
  g_interceptor->PostCmdSetDeviceMask(commandBuffer, deviceMask);
}

void InterceptPreCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
  g_interceptor->PreCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}
void InterceptPostCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
  g_interceptor->PostCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}

void InterceptPreCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
  g_interceptor->PreCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
void InterceptPostCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
  g_interceptor->PostCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void InterceptPreCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
  g_interceptor->PreCmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
void InterceptPostCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
  g_interceptor->PostCmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void InterceptPreCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
  g_interceptor->PreCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
void InterceptPostCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
  g_interceptor->PostCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

void InterceptPreCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
  g_interceptor->PreCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}
void InterceptPostCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
  g_interceptor->PostCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}

void InterceptPreSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) {
  g_interceptor->PreSetDebugUtilsObjectNameEXT(device, pNameInfo);
}
VkResult InterceptPostSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo, VkResult result) {
  return g_interceptor->PostSetDebugUtilsObjectNameEXT(device, pNameInfo, result);
}
void InterceptPreCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
  g_interceptor->PreCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
void InterceptPostCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
  g_interceptor->PostCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void InterceptPreCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
  g_interceptor->PreCmdEndDebugUtilsLabelEXT(commandBuffer);
}
void InterceptPostCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
  g_interceptor->PostCmdEndDebugUtilsLabelEXT(commandBuffer);
}

void InterceptPreCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
  g_interceptor->PreCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
void InterceptPostCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
  g_interceptor->PostCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void InterceptPreCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
  g_interceptor->PreCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
void InterceptPostCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
  g_interceptor->PostCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

void InterceptPreCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
  g_interceptor->PreCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
void InterceptPostCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
  g_interceptor->PostCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void InterceptPreCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
  g_interceptor->PreCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
void InterceptPostCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
  g_interceptor->PostCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void InterceptPreCmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PreCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
void InterceptPostCmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PostCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void InterceptPreCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PreCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
void InterceptPostCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PostCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void InterceptPreCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PreCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}
void InterceptPostCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PostCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}

void InterceptPreCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PreCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}
void InterceptPostCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
  g_interceptor->PostCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}

VkResult InterceptPostGetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue, VkResult result) {
  return g_interceptor->PostGetSemaphoreCounterValueKHR(device, semaphore, pValue, result);
}
void InterceptPreWaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
  g_interceptor->PreWaitSemaphoresKHR(device, pWaitInfo, timeout);
}
VkResult InterceptPostWaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout, VkResult result) {
  return g_interceptor->PostWaitSemaphoresKHR(device, pWaitInfo, timeout, result);
}
VkResult InterceptPostSignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo, VkResult result) {
  return g_interceptor->PostSignalSemaphoreKHR(device, pSignalInfo, result);
}
void InterceptPreCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void InterceptPostCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void InterceptPreCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void InterceptPostCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void InterceptPreCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void InterceptPostCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void InterceptPreCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void InterceptPostCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void InterceptPreCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void InterceptPostCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void InterceptPreCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void InterceptPostCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void InterceptPreCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) {
  g_interceptor->PreCmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}
void InterceptPostCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) {
  g_interceptor->PostCmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}

void InterceptPreCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
  g_interceptor->PreCmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}
void InterceptPostCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
  g_interceptor->PostCmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}

void InterceptPreCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
  g_interceptor->PreCmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}
void InterceptPostCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
  g_interceptor->PostCmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}

void InterceptPreCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
  g_interceptor->PreCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
void InterceptPostCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
  g_interceptor->PostCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void InterceptPreCmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) {
  g_interceptor->PreCmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}
void InterceptPostCmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) {
  g_interceptor->PostCmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

void InterceptPreCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) {
  g_interceptor->PreCmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}
void InterceptPostCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) {
  g_interceptor->PostCmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

void InterceptPreCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) {
  g_interceptor->PreCmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}
void InterceptPostCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) {
  g_interceptor->PostCmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}

void InterceptPreCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
  g_interceptor->PreCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}
void InterceptPostCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
  g_interceptor->PostCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}

void InterceptPreCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos) {
  g_interceptor->PreCmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}
void InterceptPostCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos) {
  g_interceptor->PostCmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}

void InterceptPreCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t** ppMaxPrimitiveCounts) {
  g_interceptor->PreCmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}
void InterceptPostCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t** ppMaxPrimitiveCounts) {
  g_interceptor->PostCmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

void InterceptPreCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
  g_interceptor->PreCmdSetCullModeEXT(commandBuffer, cullMode);
}
void InterceptPostCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
  g_interceptor->PostCmdSetCullModeEXT(commandBuffer, cullMode);
}

void InterceptPreCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
  g_interceptor->PreCmdSetFrontFaceEXT(commandBuffer, frontFace);
}
void InterceptPostCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
  g_interceptor->PostCmdSetFrontFaceEXT(commandBuffer, frontFace);
}

void InterceptPreCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
  g_interceptor->PreCmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}
void InterceptPostCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
  g_interceptor->PostCmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}

void InterceptPreCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) {
  g_interceptor->PreCmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}
void InterceptPostCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) {
  g_interceptor->PostCmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}

void InterceptPreCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) {
  g_interceptor->PreCmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}
void InterceptPostCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) {
  g_interceptor->PostCmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}

void InterceptPreCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
  g_interceptor->PreCmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
void InterceptPostCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
  g_interceptor->PostCmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void InterceptPreCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
  g_interceptor->PreCmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}
void InterceptPostCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
  g_interceptor->PostCmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}

void InterceptPreCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
  g_interceptor->PreCmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}
void InterceptPostCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
  g_interceptor->PostCmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}

void InterceptPreCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
  g_interceptor->PreCmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}
void InterceptPostCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
  g_interceptor->PostCmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}

void InterceptPreCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
  g_interceptor->PreCmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}
void InterceptPostCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
  g_interceptor->PostCmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}

void InterceptPreCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
  g_interceptor->PreCmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}
void InterceptPostCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
  g_interceptor->PostCmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}

void InterceptPreCmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
  g_interceptor->PreCmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
void InterceptPostCmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
  g_interceptor->PostCmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void InterceptPreCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) {
  g_interceptor->PreCmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}
void InterceptPostCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) {
  g_interceptor->PostCmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}

void InterceptPreCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
  g_interceptor->PreCmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}
void InterceptPostCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
  g_interceptor->PostCmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}

void InterceptPreCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
  g_interceptor->PreCmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}
void InterceptPostCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
  g_interceptor->PostCmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}

void InterceptPreCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) {
  g_interceptor->PreCmdSetLogicOpEXT(commandBuffer, logicOp);
}
void InterceptPostCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) {
  g_interceptor->PostCmdSetLogicOpEXT(commandBuffer, logicOp);
}

void InterceptPreCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
  g_interceptor->PreCmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}
void InterceptPostCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
  g_interceptor->PostCmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}

void InterceptPreCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo) {
  g_interceptor->PreCmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}
void InterceptPostCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo) {
  g_interceptor->PostCmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}

void InterceptPreCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo) {
  g_interceptor->PreCmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}
void InterceptPostCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo) {
  g_interceptor->PostCmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}

void InterceptPreCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo) {
  g_interceptor->PreCmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}
void InterceptPostCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo) {
  g_interceptor->PostCmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}

void InterceptPreCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo) {
  g_interceptor->PreCmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}
void InterceptPostCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo) {
  g_interceptor->PostCmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}

void InterceptPreCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo) {
  g_interceptor->PreCmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}
void InterceptPostCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo) {
  g_interceptor->PostCmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}

void InterceptPreCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo) {
  g_interceptor->PreCmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}
void InterceptPostCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo) {
  g_interceptor->PostCmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}

void InterceptPreCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) {
  g_interceptor->PreCmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}
void InterceptPostCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) {
  g_interceptor->PostCmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}

void InterceptPreCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
  g_interceptor->PreCmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}
void InterceptPostCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
  g_interceptor->PostCmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

void InterceptPreCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables) {
  g_interceptor->PreCmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}
void InterceptPostCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables) {
  g_interceptor->PostCmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}

void InterceptPreCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfoKHR* pDependencyInfo) {
  g_interceptor->PreCmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}
void InterceptPostCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfoKHR* pDependencyInfo) {
  g_interceptor->PostCmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}

void InterceptPreCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2KHR stageMask) {
  g_interceptor->PreCmdResetEvent2KHR(commandBuffer, event, stageMask);
}
void InterceptPostCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2KHR stageMask) {
  g_interceptor->PostCmdResetEvent2KHR(commandBuffer, event, stageMask);
}

void InterceptPreCmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfoKHR* pDependencyInfos) {
  g_interceptor->PreCmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
void InterceptPostCmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfoKHR* pDependencyInfos) {
  g_interceptor->PostCmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

void InterceptPreCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfoKHR* pDependencyInfo) {
  g_interceptor->PreCmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}
void InterceptPostCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfoKHR* pDependencyInfo) {
  g_interceptor->PostCmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}

void InterceptPreCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkQueryPool queryPool, uint32_t query) {
  g_interceptor->PreCmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}
void InterceptPostCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkQueryPool queryPool, uint32_t query) {
  g_interceptor->PostCmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}

void InterceptPreCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
  g_interceptor->PreCmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}
void InterceptPostCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
  g_interceptor->PostCmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}

void InterceptPreCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
  g_interceptor->PreCreateInstance(pCreateInfo, pAllocator, pInstance);
}
VkResult InterceptPostCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance, VkResult result) {
  return g_interceptor->PostCreateInstance(pCreateInfo, pAllocator, pInstance, result);
}
VkResult InterceptPostCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice, VkResult result) {
  return g_interceptor->PostCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice, result);
}
void InterceptPreDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PreDestroyDevice(device, pAllocator);
}
void InterceptPostDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PostDestroyDevice(device, pAllocator);
}

void InterceptPostGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) {
  g_interceptor->PostGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}

void InterceptPreQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) {
  g_interceptor->PreQueueSubmit(queue, submitCount, pSubmits, fence);
}
VkResult InterceptPostQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence, VkResult result) {
  return g_interceptor->PostQueueSubmit(queue, submitCount, pSubmits, fence, result);
}
VkResult InterceptPostQueueWaitIdle(VkQueue queue, VkResult result) {
  return g_interceptor->PostQueueWaitIdle(queue, result);
}
VkResult InterceptPostDeviceWaitIdle(VkDevice device, VkResult result) {
  return g_interceptor->PostDeviceWaitIdle(device, result);
}
VkResult InterceptPostQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence, VkResult result) {
  return g_interceptor->PostQueueBindSparse(queue, bindInfoCount, pBindInfo, fence, result);
}
VkResult InterceptPostGetFenceStatus(VkDevice device, VkFence fence, VkResult result) {
  return g_interceptor->PostGetFenceStatus(device, fence, result);
}
VkResult InterceptPostWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout, VkResult result) {
  return g_interceptor->PostWaitForFences(device, fenceCount, pFences, waitAll, timeout, result);
}
VkResult InterceptPostCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore, VkResult result) {
  return g_interceptor->PostCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore, result);
}
void InterceptPostDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PostDestroySemaphore(device, semaphore, pAllocator);
}

VkResult InterceptPostGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags, VkResult result) {
  return g_interceptor->PostGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags, result);
}
VkResult InterceptPostCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule, VkResult result) {
  return g_interceptor->PostCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule, result);
}
void InterceptPostDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PostDestroyShaderModule(device, shaderModule, pAllocator);
}

VkResult InterceptPostCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines, VkResult result) {
  return g_interceptor->PostCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, result);
}
VkResult InterceptPostCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines, VkResult result) {
  return g_interceptor->PostCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, result);
}
void InterceptPostDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PostDestroyPipeline(device, pipeline, pAllocator);
}

VkResult InterceptPostCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool, VkResult result) {
  return g_interceptor->PostCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool, result);
}
void InterceptPreDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PreDestroyCommandPool(device, commandPool, pAllocator);
}
void InterceptPostDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) {
  g_interceptor->PostDestroyCommandPool(device, commandPool, pAllocator);
}

void InterceptPreResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
  g_interceptor->PreResetCommandPool(device, commandPool, flags);
}
VkResult InterceptPostResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags, VkResult result) {
  return g_interceptor->PostResetCommandPool(device, commandPool, flags, result);
}
VkResult InterceptPostAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers, VkResult result) {
  return g_interceptor->PostAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers, result);
}
void InterceptPostFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
  g_interceptor->PostFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}

void InterceptPreBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) {
  g_interceptor->PreBeginCommandBuffer(commandBuffer, pBeginInfo);
}
VkResult InterceptPostBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo, VkResult result) {
  return g_interceptor->PostBeginCommandBuffer(commandBuffer, pBeginInfo, result);
}
void InterceptPreEndCommandBuffer(VkCommandBuffer commandBuffer) {
  g_interceptor->PreEndCommandBuffer(commandBuffer);
}
VkResult InterceptPostEndCommandBuffer(VkCommandBuffer commandBuffer, VkResult result) {
  return g_interceptor->PostEndCommandBuffer(commandBuffer, result);
}
void InterceptPreResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
  g_interceptor->PreResetCommandBuffer(commandBuffer, flags);
}
VkResult InterceptPostResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags, VkResult result) {
  return g_interceptor->PostResetCommandBuffer(commandBuffer, flags, result);
}
void InterceptPreCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
  g_interceptor->PreCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}
void InterceptPostCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
  g_interceptor->PostCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}

void InterceptPreCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
  g_interceptor->PreCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}
void InterceptPostCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
  g_interceptor->PostCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

void InterceptPreCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
  g_interceptor->PreCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}
void InterceptPostCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
  g_interceptor->PostCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

void InterceptPreCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
  g_interceptor->PreCmdSetLineWidth(commandBuffer, lineWidth);
}
void InterceptPostCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
  g_interceptor->PostCmdSetLineWidth(commandBuffer, lineWidth);
}

void InterceptPreCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
  g_interceptor->PreCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
void InterceptPostCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
  g_interceptor->PostCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void InterceptPreCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) {
  g_interceptor->PreCmdSetBlendConstants(commandBuffer, blendConstants);
}
void InterceptPostCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) {
  g_interceptor->PostCmdSetBlendConstants(commandBuffer, blendConstants);
}

void InterceptPreCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
  g_interceptor->PreCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
void InterceptPostCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
  g_interceptor->PostCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}

void InterceptPreCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
  g_interceptor->PreCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}
void InterceptPostCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
  g_interceptor->PostCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}

void InterceptPreCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
  g_interceptor->PreCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}
void InterceptPostCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
  g_interceptor->PostCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}

void InterceptPreCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
  g_interceptor->PreCmdSetStencilReference(commandBuffer, faceMask, reference);
}
void InterceptPostCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
  g_interceptor->PostCmdSetStencilReference(commandBuffer, faceMask, reference);
}

void InterceptPreCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
  g_interceptor->PreCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
void InterceptPostCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
  g_interceptor->PostCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void InterceptPreCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
  g_interceptor->PreCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}
void InterceptPostCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
  g_interceptor->PostCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

void InterceptPreCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
  g_interceptor->PreCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
void InterceptPostCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
  g_interceptor->PostCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void InterceptPreCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
  g_interceptor->PreCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
void InterceptPostCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
  g_interceptor->PostCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void InterceptPreCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
  g_interceptor->PreCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
void InterceptPostCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
  g_interceptor->PostCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void InterceptPreCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
void InterceptPostCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void InterceptPreCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
  g_interceptor->PreCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
void InterceptPostCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
  g_interceptor->PostCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void InterceptPreCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
  g_interceptor->PreCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
void InterceptPostCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
  g_interceptor->PostCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void InterceptPreCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
  g_interceptor->PreCmdDispatchIndirect(commandBuffer, buffer, offset);
}
void InterceptPostCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
  g_interceptor->PostCmdDispatchIndirect(commandBuffer, buffer, offset);
}

void InterceptPreCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
  g_interceptor->PreCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
void InterceptPostCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
  g_interceptor->PostCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void InterceptPreCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
  g_interceptor->PreCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
void InterceptPostCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
  g_interceptor->PostCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void InterceptPreCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
  g_interceptor->PreCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
void InterceptPostCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
  g_interceptor->PostCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void InterceptPreCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
  g_interceptor->PreCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
void InterceptPostCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
  g_interceptor->PostCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void InterceptPreCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
  g_interceptor->PreCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
void InterceptPostCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
  g_interceptor->PostCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void InterceptPreCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
  g_interceptor->PreCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
void InterceptPostCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
  g_interceptor->PostCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void InterceptPreCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
  g_interceptor->PreCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
void InterceptPostCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
  g_interceptor->PostCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}

void InterceptPreCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
  g_interceptor->PreCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
void InterceptPostCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
  g_interceptor->PostCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void InterceptPreCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
  g_interceptor->PreCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
void InterceptPostCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
  g_interceptor->PostCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void InterceptPreCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
  g_interceptor->PreCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
void InterceptPostCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
  g_interceptor->PostCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void InterceptPreCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
  g_interceptor->PreCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
void InterceptPostCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
  g_interceptor->PostCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void InterceptPreCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
  g_interceptor->PreCmdSetEvent(commandBuffer, event, stageMask);
}
void InterceptPostCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
  g_interceptor->PostCmdSetEvent(commandBuffer, event, stageMask);
}

void InterceptPreCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
  g_interceptor->PreCmdResetEvent(commandBuffer, event, stageMask);
}
void InterceptPostCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
  g_interceptor->PostCmdResetEvent(commandBuffer, event, stageMask);
}

void InterceptPreCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
  g_interceptor->PreCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
void InterceptPostCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
  g_interceptor->PostCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void InterceptPreCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
  g_interceptor->PreCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
void InterceptPostCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
  g_interceptor->PostCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void InterceptPreCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
  g_interceptor->PreCmdBeginQuery(commandBuffer, queryPool, query, flags);
}
void InterceptPostCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
  g_interceptor->PostCmdBeginQuery(commandBuffer, queryPool, query, flags);
}

void InterceptPreCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
  g_interceptor->PreCmdEndQuery(commandBuffer, queryPool, query);
}
void InterceptPostCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
  g_interceptor->PostCmdEndQuery(commandBuffer, queryPool, query);
}

void InterceptPreCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
  g_interceptor->PreCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
void InterceptPostCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
  g_interceptor->PostCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}

void InterceptPreCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
  g_interceptor->PreCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}
void InterceptPostCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
  g_interceptor->PostCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}

void InterceptPreCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
  g_interceptor->PreCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
void InterceptPostCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
  g_interceptor->PostCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void InterceptPreCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
  g_interceptor->PreCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}
void InterceptPostCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
  g_interceptor->PostCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void InterceptPreCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
  g_interceptor->PreCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}
void InterceptPostCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
  g_interceptor->PostCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

void InterceptPreCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
  g_interceptor->PreCmdNextSubpass(commandBuffer, contents);
}
void InterceptPostCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
  g_interceptor->PostCmdNextSubpass(commandBuffer, contents);
}

void InterceptPreCmdEndRenderPass(VkCommandBuffer commandBuffer) {
  g_interceptor->PreCmdEndRenderPass(commandBuffer);
}
void InterceptPostCmdEndRenderPass(VkCommandBuffer commandBuffer) {
  g_interceptor->PostCmdEndRenderPass(commandBuffer);
}

void InterceptPreCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
  g_interceptor->PreCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}
void InterceptPostCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
  g_interceptor->PostCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}


#ifdef VK_USE_PLATFORM_GGP 

#endif // VK_USE_PLATFORM_GGP
// clang-format on
