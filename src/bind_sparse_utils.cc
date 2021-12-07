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

#include "bind_sparse_utils.h"

#include <sstream>

#include "util.h"

namespace GFR {

bool BindSparseUtils::BindSparseInfoWaitsOnBinarySemaphores(
    const VkBindSparseInfo* bind_info,
    const SemaphoreTracker* semaphore_tracker) {
  assert(semaphore_tracker);
  if (!semaphore_tracker || !bind_info->waitSemaphoreCount) {
    return false;
  }
  for (uint32_t i = 0; i < bind_info->waitSemaphoreCount; i++) {
    if (semaphore_tracker->GetSemaphoreType(bind_info->pWaitSemaphores[i]) ==
        VK_SEMAPHORE_TYPE_BINARY_KHR) {
      return true;
    }
  }
  return false;
}

bool BindSparseUtils::ShouldExpandQueueBindSparseToTrackSemaphores(
    const PackedBindSparseInfo* packed_bind_sparse_info) {
  // If any of the VkBindSparseInfos signals sempahores or waits on binary
  // semaphores, we need to expand vkQueueBindSparse call to a proper
  // combination of vkQueueBindSparse and vkQueueSubmit calls.
  for (uint32_t i = 0; i < packed_bind_sparse_info->bind_info_count; i++) {
    if (packed_bind_sparse_info->bind_infos[i].signalSemaphoreCount > 0) {
      return true;
    }
  }
  for (uint32_t i = 0; i < packed_bind_sparse_info->bind_info_count; i++) {
    if (BindSparseInfoWaitsOnBinarySemaphores(
            &packed_bind_sparse_info->bind_infos[i],
            packed_bind_sparse_info->semaphore_tracker)) {
      return true;
    }
  }
  return false;
}

void BindSparseUtils::GetWaitBinarySemaphores(
    const VkBindSparseInfo* bind_info,
    const SemaphoreTracker* semaphore_tracker,
    std::vector<VkSemaphore>* wait_binary_semaphores) {
  assert(semaphore_tracker);
  if (!semaphore_tracker || !bind_info->waitSemaphoreCount) {
    return;
  }
  for (uint32_t i = 0; i < bind_info->waitSemaphoreCount; i++) {
    if (semaphore_tracker->GetSemaphoreType(bind_info->pWaitSemaphores[i]) ==
        VK_SEMAPHORE_TYPE_BINARY_KHR) {
      wait_binary_semaphores->push_back(bind_info->pWaitSemaphores[i]);
    }
  }
  return;
}

// Given an array of VkBindSparseInfo objects, originally passed to
// vkQueueBindSparse, this function generates a list of vkQueueBindSparse
// operations interleaved with vkQueueSubmits. The idea is to add a
// vkQueueSubmit after each BindSparseInfo that signals some semaphores, so we
// can update the values of the markers associated to that semaphores
// accordingly.
void BindSparseUtils::ExpandBindSparseInfo(
    ExpandedBindSparseInfo* bind_sparse_expand_info) {
  bind_sparse_expand_info->submit_infos.clear();
  bind_sparse_expand_info->timeline_semaphore_infos.clear();
  VkSubmitInfo vk_submit_info = {};
  vk_submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkTimelineSemaphoreSubmitInfoKHR vk_timeline_semaphore_submit_info = {};
  vk_timeline_semaphore_submit_info.sType =
      VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR;
  auto& packed_bind_sparse_info =
      bind_sparse_expand_info->packed_bind_sparse_info;
  for (uint32_t i = 0; i < packed_bind_sparse_info->bind_info_count; i++) {
    const VkTimelineSemaphoreSubmitInfoKHR* timeline_semaphore_info =
        FindOnChain<VkTimelineSemaphoreSubmitInfoKHR,
                    VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR>(
            packed_bind_sparse_info->bind_infos[i].pNext);

    // If the bind info has a wait operation on a binary semaphore do a
    // vkQueueSubmit.
    if (BindSparseInfoWaitsOnBinarySemaphores(
            &packed_bind_sparse_info->bind_infos[i],
            packed_bind_sparse_info->semaphore_tracker)) {
      bind_sparse_expand_info->queue_operation_types.push_back(kQueueSubmit);
      bind_sparse_expand_info->submit_infos.push_back(vk_submit_info);
      auto& submit_info = bind_sparse_expand_info->submit_infos.back();
      if (timeline_semaphore_info) {
        bind_sparse_expand_info->has_timeline_semaphore_info.push_back(true);
        bind_sparse_expand_info->timeline_semaphore_infos.push_back(
            vk_timeline_semaphore_submit_info);
        auto& tsinfo = bind_sparse_expand_info->timeline_semaphore_infos.back();
        tsinfo.waitSemaphoreValueCount =
            timeline_semaphore_info->waitSemaphoreValueCount;
        tsinfo.pWaitSemaphoreValues =
            timeline_semaphore_info->pWaitSemaphoreValues;
      } else {
        bind_sparse_expand_info->has_timeline_semaphore_info.push_back(false);
      }
      submit_info.waitSemaphoreCount =
          packed_bind_sparse_info->bind_infos[i].waitSemaphoreCount;
      submit_info.pWaitSemaphores =
          packed_bind_sparse_info->bind_infos[i].pWaitSemaphores;
      // Since waiting on a binary semaphore resets the semaphore, we signal
      // the binary semaphores again. We keep track of those semaphores here
      // and add them to the submit before consumption to avoid issues related
      // to pointing to vector members that may resize.
      bind_sparse_expand_info->wait_binary_semaphores.push_back(
          std::vector<VkSemaphore>());
      auto& wbsemaphores =
          bind_sparse_expand_info->wait_binary_semaphores.back();
      GetWaitBinarySemaphores(&packed_bind_sparse_info->bind_infos[i],
                              packed_bind_sparse_info->semaphore_tracker,
                              &wbsemaphores);
    }

    // Always do the sparse binding.
    bind_sparse_expand_info->queue_operation_types.push_back(kQueueBindSparse);

    // If the bind info has a signal operation on a semaphore do a
    // vkQueueSubmit.
    if (packed_bind_sparse_info->bind_infos[i].signalSemaphoreCount > 0) {
      bind_sparse_expand_info->queue_operation_types.push_back(kQueueSubmit);
      bind_sparse_expand_info->submit_infos.push_back(vk_submit_info);
      auto& submit_info = bind_sparse_expand_info->submit_infos.back();
      if (timeline_semaphore_info) {
        bind_sparse_expand_info->has_timeline_semaphore_info.push_back(true);
        bind_sparse_expand_info->timeline_semaphore_infos.push_back(
            vk_timeline_semaphore_submit_info);
        auto& tsinfo = bind_sparse_expand_info->timeline_semaphore_infos.back();
        tsinfo.signalSemaphoreValueCount =
            timeline_semaphore_info->signalSemaphoreValueCount;
        tsinfo.pSignalSemaphoreValues =
            timeline_semaphore_info->pSignalSemaphoreValues;
      } else {
        bind_sparse_expand_info->has_timeline_semaphore_info.push_back(false);
      }
      submit_info.signalSemaphoreCount =
          packed_bind_sparse_info->bind_infos[i].signalSemaphoreCount;
      submit_info.pSignalSemaphores =
          packed_bind_sparse_info->bind_infos[i].pSignalSemaphores;
    }
  }
}

std::string BindSparseUtils::LogBindSparseInfosSemaphores(
    const Device* device, VkDevice vk_device, VkQueue vk_queue,
    uint32_t bind_info_count, const VkBindSparseInfo* bind_infos) {
  std::stringstream log;
  bool msg_header_printed = false;

  auto semaphore_tracker = device->GetSemaphoreTracker();
  std::vector<VkSemaphore> wait_semaphores, signal_semaphores;
  std::vector<uint64_t> wait_semaphore_values, signal_semaphore_values;

  for (uint32_t i = 0; i < bind_info_count; i++) {
    if (bind_infos[i].waitSemaphoreCount == 0 &&
        bind_infos[i].signalSemaphoreCount == 0) {
      continue;
    }
    if (!msg_header_printed) {
      log << "[GFR] VkBindSparseInfo with semaphores submitted to queue:";
      log << "\n   VkDevice:" << device->GetObjectName((uint64_t)vk_device)
          << "\n   VkQueue: " << device->GetObjectName((uint64_t)vk_queue)
          << std::endl;
    }

    wait_semaphores.clear();
    signal_semaphores.clear();
    for (uint32_t j = 0; j < bind_infos[i].waitSemaphoreCount; j++) {
      wait_semaphores.push_back(bind_infos[i].pWaitSemaphores[j]);
      wait_semaphore_values.push_back(0);
    }
    for (uint32_t j = 0; j < bind_infos[i].signalSemaphoreCount; j++) {
      signal_semaphores.push_back(bind_infos[i].pSignalSemaphores[j]);
      signal_semaphore_values.push_back(1);
    }

    const VkTimelineSemaphoreSubmitInfoKHR* timeline_semaphore_info =
        FindOnChain<VkTimelineSemaphoreSubmitInfoKHR,
                    VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR>(
            bind_infos[i].pNext);

    if (timeline_semaphore_info) {
      for (uint32_t j = 0; j < timeline_semaphore_info->waitSemaphoreValueCount;
           j++) {
        if (semaphore_tracker->GetSemaphoreType(wait_semaphores[j]) ==
            VK_SEMAPHORE_TYPE_TIMELINE_KHR) {
          wait_semaphore_values[j] =
              timeline_semaphore_info->pWaitSemaphoreValues[j];
        }
      }
      for (uint32_t j = 0;
           j < timeline_semaphore_info->signalSemaphoreValueCount; j++) {
        if (semaphore_tracker->GetSemaphoreType(signal_semaphores[j]) ==
            VK_SEMAPHORE_TYPE_TIMELINE_KHR) {
          signal_semaphore_values[j] =
              timeline_semaphore_info->pSignalSemaphoreValues[j];
        }
      }
    }

    const char* tab = "\t";
    log << tab << "****** QueueBindSparse #" << i << " ******\n";
    if (wait_semaphores.size() > 0) {
      log << tab << "*** Wait Semaphores ***\n";
      const auto& tracked_semaphore_infos =
          semaphore_tracker->GetTrackedSemaphoreInfos(wait_semaphores,
                                                      wait_semaphore_values);
      log << semaphore_tracker->PrintTrackedSemaphoreInfos(
          tracked_semaphore_infos, tab);
    }
    if (signal_semaphores.size() > 0) {
      log << tab << "*** Signal Semaphores ***\n";
      const auto& tracked_semaphore_infos =
          semaphore_tracker->GetTrackedSemaphoreInfos(signal_semaphores,
                                                      signal_semaphore_values);
      log << semaphore_tracker->PrintTrackedSemaphoreInfos(
          tracked_semaphore_infos, tab);
    }
  }
  return log.str();
}

}  // namespace GFR
