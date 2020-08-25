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

#include <sstream>

#include "gfr.h"
#include "pipeline.h"
#include "util.h"

namespace gfr {

PipelineBoundShader PipelineBoundShader::NULL_SHADER = {
    static_cast<VkShaderStageFlagBits>(0), VK_NULL_HANDLE, "<NULL>"};

// =================================================================================================
// Pipeline
// =================================================================================================
Pipeline::Pipeline(VkPipeline vk_pipeline,
                   const VkGraphicsPipelineCreateInfo& graphics_create_info)
    : vk_pipeline_(vk_pipeline),
      pipeline_bind_point_(VK_PIPELINE_BIND_POINT_GRAPHICS) {
  InitFromShaderStages(graphics_create_info.pStages,
                       graphics_create_info.stageCount);
}

Pipeline::Pipeline(VkPipeline vk_pipeline,
                   const VkComputePipelineCreateInfo& compute_create_info)
    : vk_pipeline_(vk_pipeline),
      pipeline_bind_point_(VK_PIPELINE_BIND_POINT_COMPUTE) {
  InitFromShaderStages(&compute_create_info.stage, 1);
}

void Pipeline::InitFromShaderStages(
    const VkPipelineShaderStageCreateInfo* stages, uint32_t stage_count) {
  for (uint32_t shader_index = 0; shader_index < stage_count; ++shader_index) {
    PipelineBoundShader shader_stage = {stages[shader_index].stage,
                                        stages[shader_index].module,
                                        stages[shader_index].pName};

    shaders_.push_back(shader_stage);
  }
}

const PipelineBoundShader& Pipeline::FindShaderStage(
    VkShaderStageFlagBits shader_stage) const {
  for (const auto& shader : shaders_) {
    if (shader.stage == shader_stage) {
      return shader;
    }
  }

  return PipelineBoundShader::NULL_SHADER;
}

std::ostream& Pipeline::PrintName(std::ostream& stream,
                                  const ObjectInfoDB& name_resolver,
                                  const std::string& indent) const {
  stream << name_resolver.GetObjectInfo((uint64_t)vk_pipeline_, indent);
  auto bind_point = GetVkPipelineBindPoint();
  stream << indent << "bindPoint: ";
  if (bind_point == VK_PIPELINE_BIND_POINT_GRAPHICS) {
    stream << "graphics";
  } else if (bind_point == VK_PIPELINE_BIND_POINT_COMPUTE) {
    stream << "compute";
  } else {
    stream << "unknown";
  }
  return stream;
}

std::ostream& Pipeline::Print(std::ostream& stream,
                              const ObjectInfoDB& name_resolver,
                              const std::string& indent) const {
  auto indent1 = gfr::IncreaseIndent(indent);
  PrintName(stream, name_resolver, indent1);

  const auto num_shaders = shaders_.size();
  if (num_shaders) {
    auto indent2 = gfr::IncreaseIndent(indent1);
    auto indent3 = gfr::IncreaseIndent(indent2);
    stream << indent1 << "shaderInfos:";
    for (auto shader_index = 0u; shader_index < num_shaders; ++shader_index) {
      auto const& shader = shaders_[shader_index];
      stream << indent2 << "- # shaderInfo:";
      stream << indent3 << "stage: ";
      if (shader.stage == VK_SHADER_STAGE_VERTEX_BIT) {
        stream << "vs";
      } else if (shader.stage == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) {
        stream << "tc";
      } else if (shader.stage == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) {
        stream << "te";
      } else if (shader.stage == VK_SHADER_STAGE_GEOMETRY_BIT) {
        stream << "gs";
      } else if (shader.stage == VK_SHADER_STAGE_FRAGMENT_BIT) {
        stream << "fs";
      } else if (shader.stage == VK_SHADER_STAGE_COMPUTE_BIT) {
        stream << "cs";
      }
      stream << indent3 << "module: ";
      stream << name_resolver.GetObjectName((uint64_t)shader.module);
      stream << indent3 << "entry: ";
      stream << "\"" << shader.entry_point << "\"";
    }
  }

  return stream;
}

VkPipeline Pipeline::GetVkPipeline() const { return vk_pipeline_; }

VkPipelineBindPoint Pipeline::GetVkPipelineBindPoint() const {
  return pipeline_bind_point_;
}

}  // namespace gfr
