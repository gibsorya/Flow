#ifndef FLOW_DATA
#define FLOW_DATA

#include <vulkan/vulkan.hpp>
#include <vector>

/*
* Represents all the data for a single Vulkan Graphics Pipeline.
* 
* This struct is meant to be used to store the information that is passed into the createGraphicsPipeline() function, so that the pipeline can be customized before creation.
*/
struct GraphicsPipelineData
{
    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages; //Stores all the shader data you need
    vk::PipelineShaderStageCreateInfo vertexShaderInfo;
    vk::PipelineShaderStageCreateInfo fragShaderInfo;

    vk::PipelineVertexInputStateCreateInfo vertexInput;
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    vk::PipelineTessellationStateCreateInfo tessallationState;
    vk::PipelineViewportStateCreateInfo viewportState;
    vk::PipelineRasterizationStateCreateInfo rasterization;
    vk::PipelineMultisampleStateCreateInfo multisampler;
    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    vk::PipelineColorBlendStateCreateInfo colorBlend;
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    vk::PipelineDynamicStateCreateInfo dynamicState;

    vk::PipelineLayout layout;
    vk::RenderPass renderPass;
    uint32_t subpass;
};

#endif