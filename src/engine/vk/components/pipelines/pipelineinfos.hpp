#ifndef PIPELINE_INFOS
#define PIPELINE_INFOS

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>

namespace flow::vulkan::pipelines{
    void populateVertexInputInfo(vk::PipelineVertexInputStateCreateInfo &createInfo);
    void populateInputAssemblyInfo(vk::PipelineInputAssemblyStateCreateInfo &createInfo);
    void populateTesselationInfo(vk::PipelineTessellationStateCreateInfo &createInfo);
    void populateViewportInfo(vk::PipelineViewportStateCreateInfo &createInfo, vk::Rect2D scissor, vk::Viewport viewport);
    void populateRasterizationInfo(vk::PipelineRasterizationStateCreateInfo &createInfo);
    void populateMultisamplerInfo(vk::PipelineMultisampleStateCreateInfo &createInfo);
    void populateDepthStencilInfo(vk::PipelineDepthStencilStateCreateInfo &createInfo);
    void populateColorBlendAttachmentInfo(vk::PipelineColorBlendAttachmentState &createInfo);
    void populateColorBlendInfo(vk::PipelineColorBlendStateCreateInfo &createInfo, vk::PipelineColorBlendAttachmentState attachment);
    void populateDynamicStateInfo(vk::PipelineDynamicStateCreateInfo &createInfo);
    

    void populateShaderStageInfo(vk::PipelineShaderStageCreateInfo &createInfo, vk::ShaderModule module, vk::ShaderStageFlagBits stageBits, const char* name);
}

#endif // PIPELINE_INFOS