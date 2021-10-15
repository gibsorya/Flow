#include "pipelineinfos.hpp"

namespace flow::vulkan::pipelines{

    //Default Vertex Input Info
    void populateVertexInputInfo(vk::PipelineVertexInputStateCreateInfo &createInfo) {
        createInfo.vertexBindingDescriptionCount = 0;
        createInfo.pVertexBindingDescriptions = nullptr;
        createInfo.vertexAttributeDescriptionCount = 0;
        createInfo.pVertexAttributeDescriptions = nullptr;
    }

    //Default Input Assembly Info
    void populateInputAssemblyInfo(vk::PipelineInputAssemblyStateCreateInfo &createInfo) {
        createInfo.topology = vk::PrimitiveTopology::eTriangleList;
        createInfo.primitiveRestartEnable = VK_FALSE;
    }

    //Default Tesselation Info
    void populateTesselationInfo(vk::PipelineTessellationStateCreateInfo &createInfo) {
        // createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;    
    }

    //Default viewport info
    void populateViewportInfo(vk::PipelineViewportStateCreateInfo &createInfo, vk::Rect2D scissor, vk::Viewport viewport) {
        createInfo.scissorCount = 1;
        createInfo.pScissors = &scissor;
        createInfo.viewportCount = 1;
        createInfo.pViewports = &viewport;
    }

    //Default rasterization info
    void populateRasterizationInfo(vk::PipelineRasterizationStateCreateInfo &createInfo) {
        createInfo.depthClampEnable = VK_FALSE;
        createInfo.rasterizerDiscardEnable = VK_FALSE;
        createInfo.polygonMode = vk::PolygonMode::eFill;
        createInfo.frontFace = vk::FrontFace::eCounterClockwise;
        createInfo.depthBiasEnable = VK_FALSE;
        createInfo.depthBiasConstantFactor = 0;
        createInfo.depthBiasClamp = 0;
        createInfo.depthBiasSlopeFactor = 0;
        createInfo.lineWidth = 1.0f;
    }

    //Default multisampler info
    void populateMultisamplerInfo(vk::PipelineMultisampleStateCreateInfo &createInfo) {
        createInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
        createInfo.sampleShadingEnable = VK_FALSE;
        createInfo.minSampleShading = 0.2f;
        createInfo.pSampleMask = nullptr;
        createInfo.alphaToCoverageEnable = VK_FALSE;
        createInfo.alphaToOneEnable = VK_FALSE;
    }

    //Default depth stencil info
    void populateDepthStencilInfo(vk::PipelineDepthStencilStateCreateInfo &createInfo) {
        createInfo = vk::PipelineDepthStencilStateCreateInfo({}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);
    }

    //Default color blend info
    void populateColorBlendInfo(vk::PipelineColorBlendStateCreateInfo &createInfo, vk::PipelineColorBlendAttachmentState attachment) {
        createInfo.logicOpEnable = VK_FALSE;
        createInfo.logicOp = vk::LogicOp::eCopy;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &attachment;
        createInfo.blendConstants[0] = 0.0f;
        createInfo.blendConstants[1] = 0.0f;
        createInfo.blendConstants[2] = 0.0f;
        createInfo.blendConstants[3] = 0.0f;    
    }

    //Default dynamic state info
    void populateDynamicStateInfo(vk::PipelineDynamicStateCreateInfo &createInfo) {
        /* Nothing here yet */
    }

    //Default shader stage info
    void populateShaderStageInfo(vk::PipelineShaderStageCreateInfo &createInfo, vk::ShaderModule module, vk::ShaderStageFlagBits stageBits, const char* name) {
        createInfo.stage = stageBits;
        createInfo.module = module;
        createInfo.pName = name;
    }

    //Default color blend attachment info
    void populateColorBlendAttachmentInfo(vk::PipelineColorBlendAttachmentState &createInfo){
        createInfo.blendEnable = VK_FALSE;
        createInfo.srcColorBlendFactor = vk::BlendFactor::eOne;
        createInfo.dstColorBlendFactor = vk::BlendFactor::eZero;
        createInfo.colorBlendOp = vk::BlendOp::eAdd;
        createInfo.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        createInfo.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        createInfo.alphaBlendOp = vk::BlendOp::eAdd;
        createInfo.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    }
    
}