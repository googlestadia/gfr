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

#ifndef GFR_PIPELINE_H
#define GFR_PIPELINE_H

#include <vulkan/vulkan.h>

#include <memory>
#include <string>
#include <vector>

#include "object_name_db.h"

namespace GFR {

// =================================================================================================
// Shader bound to a pipeline
// =================================================================================================
struct PipelineBoundShader {
  VkShaderStageFlagBits stage;
  VkShaderModule module;
  std::string entry_point;

  static PipelineBoundShader NULL_SHADER;
};

// =================================================================================================
// Pipeline
// =================================================================================================
class Pipeline {
 public:
  Pipeline(VkPipeline vk_pipeline,
           const VkGraphicsPipelineCreateInfo& graphics_create_info);

  Pipeline(VkPipeline vk_pipeline,
           const VkComputePipelineCreateInfo& compute_create_info);

  VkPipeline GetVkPipeline() const;
  VkPipelineBindPoint GetVkPipelineBindPoint() const;

  const PipelineBoundShader& FindShaderStage(
      VkShaderStageFlagBits shader_stage) const;

  std::ostream& PrintName(std::ostream& stream,
                          const ObjectInfoDB& name_resolver,
                          const std::string& indent) const;
  std::ostream& Print(std::ostream& stream, const ObjectInfoDB& name_resolver,
                      const std::string& indent) const;

  const std::vector<PipelineBoundShader>& GetBoundShaders() const {
    return shaders_;
  }

 private:
  void InitFromShaderStages(const VkPipelineShaderStageCreateInfo* stages,
                            uint32_t stage_count);

 protected:
  VkPipeline vk_pipeline_;
  VkPipelineBindPoint pipeline_bind_point_ =
      static_cast<VkPipelineBindPoint>(UINT32_MAX);
  std::vector<PipelineBoundShader> shaders_;
};

using PipelinePtr = std::shared_ptr<Pipeline>;

}  // namespace GFR

#endif  // GFR_PIPELINE_H
