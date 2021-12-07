
// clang-format off


void GfrContext::PreCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void GfrContext::PostCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}


void GfrContext::PreCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

void GfrContext::PostCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}


void GfrContext::PreCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}

void GfrContext::PostCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}


void GfrContext::PreCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDebugMarkerEndEXT(commandBuffer);
}

void GfrContext::PostCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDebugMarkerEndEXT(commandBuffer);
}


void GfrContext::PreCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}

void GfrContext::PostCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}


void GfrContext::PreCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void GfrContext::PostCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}


void GfrContext::PreCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDeviceMask(commandBuffer, deviceMask);
}

void GfrContext::PostCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDeviceMask(commandBuffer, deviceMask);
}


void GfrContext::PreCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}

void GfrContext::PostCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}


void GfrContext::PreCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void GfrContext::PostCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}


void GfrContext::PreCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void GfrContext::PostCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}


void GfrContext::PreCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

void GfrContext::PostCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}


void GfrContext::PreCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}

void GfrContext::PostCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}


void GfrContext::PreCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void GfrContext::PostCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}


void GfrContext::PreCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdEndDebugUtilsLabelEXT(commandBuffer);
}

void GfrContext::PostCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdEndDebugUtilsLabelEXT(commandBuffer);
}


void GfrContext::PreCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void GfrContext::PostCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}


void GfrContext::PreCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

void GfrContext::PostCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}


void GfrContext::PreCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void GfrContext::PostCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}


void GfrContext::PreCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void GfrContext::PostCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}


void GfrContext::PreCmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void GfrContext::PostCmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}


void GfrContext::PreCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void GfrContext::PostCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}


void GfrContext::PreCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}

void GfrContext::PostCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}


void GfrContext::PreCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}

void GfrContext::PostCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}


void GfrContext::PreCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void GfrContext::PostCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


void GfrContext::PreCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void GfrContext::PostCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


void GfrContext::PreCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void GfrContext::PostCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


void GfrContext::PreCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void GfrContext::PostCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


void GfrContext::PreCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void GfrContext::PostCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


void GfrContext::PreCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void GfrContext::PostCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


void GfrContext::PreCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}

void GfrContext::PostCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}


void GfrContext::PreCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}

void GfrContext::PostCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}


void GfrContext::PreCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}

void GfrContext::PostCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}


void GfrContext::PreCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void GfrContext::PostCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}


void GfrContext::PreCmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

void GfrContext::PostCmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}


void GfrContext::PreCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

void GfrContext::PostCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}


void GfrContext::PreCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}

void GfrContext::PostCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}


void GfrContext::PreCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}

void GfrContext::PostCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}


void GfrContext::PreCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}

void GfrContext::PostCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}


void GfrContext::PreCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t** ppMaxPrimitiveCounts) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

void GfrContext::PostCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t** ppMaxPrimitiveCounts) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}


void GfrContext::PreCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetCullModeEXT(commandBuffer, cullMode);
}

void GfrContext::PostCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetCullModeEXT(commandBuffer, cullMode);
}


void GfrContext::PreCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetFrontFaceEXT(commandBuffer, frontFace);
}

void GfrContext::PostCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetFrontFaceEXT(commandBuffer, frontFace);
}


void GfrContext::PreCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}

void GfrContext::PostCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}


void GfrContext::PreCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}

void GfrContext::PostCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}


void GfrContext::PreCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}

void GfrContext::PostCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}


void GfrContext::PreCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void GfrContext::PostCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}


void GfrContext::PreCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}

void GfrContext::PostCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}


void GfrContext::PreCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}

void GfrContext::PostCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}


void GfrContext::PreCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}

void GfrContext::PostCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}


void GfrContext::PreCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}

void GfrContext::PostCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}


void GfrContext::PreCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}

void GfrContext::PostCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}


void GfrContext::PreCmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void GfrContext::PostCmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}


void GfrContext::PreCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}

void GfrContext::PostCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}


void GfrContext::PreCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}

void GfrContext::PostCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}


void GfrContext::PreCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}

void GfrContext::PostCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}


void GfrContext::PreCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetLogicOpEXT(commandBuffer, logicOp);
}

void GfrContext::PostCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetLogicOpEXT(commandBuffer, logicOp);
}


void GfrContext::PreCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}

void GfrContext::PostCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}


void GfrContext::PreCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}

void GfrContext::PostCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}


void GfrContext::PreCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}

void GfrContext::PostCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}


void GfrContext::PreCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}

void GfrContext::PostCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}


void GfrContext::PreCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}

void GfrContext::PostCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}


void GfrContext::PreCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}

void GfrContext::PostCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}


void GfrContext::PreCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}

void GfrContext::PostCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}


void GfrContext::PreCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}

void GfrContext::PostCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}


void GfrContext::PreCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

void GfrContext::PostCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}


void GfrContext::PreCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}

void GfrContext::PostCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}


void GfrContext::PreCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfoKHR* pDependencyInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}

void GfrContext::PostCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfoKHR* pDependencyInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}


void GfrContext::PreCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2KHR stageMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdResetEvent2KHR(commandBuffer, event, stageMask);
}

void GfrContext::PostCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2KHR stageMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdResetEvent2KHR(commandBuffer, event, stageMask);
}


void GfrContext::PreCmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfoKHR* pDependencyInfos) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

void GfrContext::PostCmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfoKHR* pDependencyInfos) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}


void GfrContext::PreCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfoKHR* pDependencyInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}

void GfrContext::PostCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfoKHR* pDependencyInfo) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}


void GfrContext::PreCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkQueryPool queryPool, uint32_t query) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}

void GfrContext::PostCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkQueryPool queryPool, uint32_t query) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}


void GfrContext::PreCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}

void GfrContext::PostCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}



VkResult GfrContext::PostBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo, VkResult result) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostBeginCommandBuffer(commandBuffer, pBeginInfo, result);
}

VkResult GfrContext::PreEndCommandBuffer(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreEndCommandBuffer(commandBuffer);
}

VkResult GfrContext::PostEndCommandBuffer(VkCommandBuffer commandBuffer, VkResult result) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostEndCommandBuffer(commandBuffer, result);
}


VkResult GfrContext::PostResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags, VkResult result) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostResetCommandBuffer(commandBuffer, flags, result);
}


void GfrContext::PostCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}


void GfrContext::PreCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

void GfrContext::PostCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}


void GfrContext::PreCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

void GfrContext::PostCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}


void GfrContext::PreCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetLineWidth(commandBuffer, lineWidth);
}

void GfrContext::PostCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetLineWidth(commandBuffer, lineWidth);
}


void GfrContext::PreCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void GfrContext::PostCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}


void GfrContext::PreCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetBlendConstants(commandBuffer, blendConstants);
}

void GfrContext::PostCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetBlendConstants(commandBuffer, blendConstants);
}


void GfrContext::PreCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}

void GfrContext::PostCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}


void GfrContext::PreCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}

void GfrContext::PostCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}


void GfrContext::PreCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}

void GfrContext::PostCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}


void GfrContext::PreCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetStencilReference(commandBuffer, faceMask, reference);
}

void GfrContext::PostCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetStencilReference(commandBuffer, faceMask, reference);
}


void GfrContext::PreCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void GfrContext::PostCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}


void GfrContext::PreCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

void GfrContext::PostCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}


void GfrContext::PreCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void GfrContext::PostCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}


void GfrContext::PreCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void GfrContext::PostCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}


void GfrContext::PreCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void GfrContext::PostCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}


void GfrContext::PreCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void GfrContext::PostCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}


void GfrContext::PreCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void GfrContext::PostCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}


void GfrContext::PreCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void GfrContext::PostCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}


void GfrContext::PreCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdDispatchIndirect(commandBuffer, buffer, offset);
}

void GfrContext::PostCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdDispatchIndirect(commandBuffer, buffer, offset);
}


void GfrContext::PreCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void GfrContext::PostCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}


void GfrContext::PreCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void GfrContext::PostCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}


void GfrContext::PreCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void GfrContext::PostCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}


void GfrContext::PreCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void GfrContext::PostCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}


void GfrContext::PreCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void GfrContext::PostCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}


void GfrContext::PreCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void GfrContext::PostCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}


void GfrContext::PreCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}

void GfrContext::PostCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}


void GfrContext::PreCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void GfrContext::PostCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}


void GfrContext::PreCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void GfrContext::PostCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}


void GfrContext::PreCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void GfrContext::PostCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}


void GfrContext::PreCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void GfrContext::PostCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}


void GfrContext::PreCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdSetEvent(commandBuffer, event, stageMask);
}

void GfrContext::PostCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdSetEvent(commandBuffer, event, stageMask);
}


void GfrContext::PreCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdResetEvent(commandBuffer, event, stageMask);
}

void GfrContext::PostCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdResetEvent(commandBuffer, event, stageMask);
}


void GfrContext::PreCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void GfrContext::PostCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}


void GfrContext::PreCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void GfrContext::PostCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}


void GfrContext::PreCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBeginQuery(commandBuffer, queryPool, query, flags);
}

void GfrContext::PostCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBeginQuery(commandBuffer, queryPool, query, flags);
}


void GfrContext::PreCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdEndQuery(commandBuffer, queryPool, query);
}

void GfrContext::PostCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdEndQuery(commandBuffer, queryPool, query);
}


void GfrContext::PreCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}

void GfrContext::PostCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}


void GfrContext::PreCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}

void GfrContext::PostCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}


void GfrContext::PreCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void GfrContext::PostCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}


void GfrContext::PreCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void GfrContext::PostCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}


void GfrContext::PreCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

void GfrContext::PostCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}


void GfrContext::PreCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdNextSubpass(commandBuffer, contents);
}

void GfrContext::PostCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdNextSubpass(commandBuffer, contents);
}


void GfrContext::PreCmdEndRenderPass(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdEndRenderPass(commandBuffer);
}

void GfrContext::PostCmdEndRenderPass(VkCommandBuffer commandBuffer) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdEndRenderPass(commandBuffer);
}


void GfrContext::PreCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PreCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}

void GfrContext::PostCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
   return GFR::GetGfrCommandBuffer(commandBuffer)->PostCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}

#ifdef VK_USE_PLATFORM_GGP 
#endif // VK_USE_PLATFORM_GGP
// clang-format on
