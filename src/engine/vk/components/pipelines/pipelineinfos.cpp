// #include "pipelineinfos.hpp"
// #include "../../root.hpp"

// namespace flow::vulkan::pipelines{

//     //Default Vertex Input Info
//     void populateVertexInputInfo(VkPipelineVertexInputStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//         createInfo.vertexBindingDescriptionCount = 0;
//         createInfo.pVertexBindingDescriptions = nullptr;
//         createInfo.vertexAttributeDescriptionCount = 0;
//         createInfo.pVertexAttributeDescriptions = nullptr;
//     }

//     //Default Input Assembly Info
//     void populateInputAssemblyInfo(VkPipelineInputAssemblyStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//         createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//         createInfo.primitiveRestartEnable = VK_FALSE;
//     }

//     void populateTesselationInfo(VkPipelineTessellationStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;    
//     }

//     void populateViewportInfo(VkPipelineViewportStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//         createInfo.scissorCount = 1;
//         createInfo.pScissors = &root.flowScissors.scissors.back();
//         createInfo.viewportCount = 1;
//         createInfo.pViewports = &root.flowViewports.viewports.back();
//     }

//     void populateRasterizationInfo(VkPipelineRasterizationStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//         createInfo.depthClampEnable = VK_FALSE;
//         createInfo.rasterizerDiscardEnable = VK_FALSE;
//         createInfo.polygonMode = VK_POLYGON_MODE_FILL;
//         createInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
//         createInfo.depthBiasEnable = VK_FALSE;
//         createInfo.depthBiasConstantFactor = 0;
//         createInfo.depthBiasClamp = 0;
//         createInfo.depthBiasSlopeFactor = 0;
//         createInfo.lineWidth = 1.0f;
//     }

//     void populateMultisamplerInfo(VkPipelineMultisampleStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//         createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//         createInfo.sampleShadingEnable = VK_FALSE;
//         createInfo.minSampleShading = 0.2f;
//         createInfo.pSampleMask = nullptr;
//         createInfo.alphaToCoverageEnable = VK_FALSE;
//         createInfo.alphaToOneEnable = VK_FALSE;
//     }

//     void populateDepthStencilInfo(VkPipelineDepthStencilStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//         createInfo.depthTestEnable = VK_TRUE;
//         createInfo.depthWriteEnable = VK_TRUE;
//         createInfo.depthCompareOp = VK_COMPARE_OP_LESS;
//         createInfo.depthBoundsTestEnable = VK_FALSE;
//         createInfo.stencilTestEnable = VK_FALSE;
//         createInfo.front = {};
//         createInfo.back = {};
//         createInfo.minDepthBounds = 0.0f;
//         createInfo.maxDepthBounds = 1.0f;
//     }

//     void populateColorBlendInfo(VkPipelineColorBlendStateCreateInfo &createInfo, VkPipelineColorBlendAttachmentState attachment) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//         createInfo.logicOpEnable = VK_FALSE;
//         createInfo.logicOp = VK_LOGIC_OP_COPY;
//         createInfo.attachmentCount = 1;
//         createInfo.pAttachments = &attachment;
//         createInfo.blendConstants[0] = 0.0f;
//         createInfo.blendConstants[1] = 0.0f;
//         createInfo.blendConstants[2] = 0.0f;
//         createInfo.blendConstants[3] = 0.0f;    
//     }

//     void populateDynamicStateInfo(VkPipelineDynamicStateCreateInfo &createInfo) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        
    
//     }

//     void populateShaderStageInfo(VkPipelineShaderStageCreateInfo &createInfo, VkShaderModule module, VkShaderStageFlagBits stageBits, const char* name) {
//         createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         createInfo.stage = stageBits;
//         createInfo.module = module;
//         createInfo.pName = name;
//     }

//     void populateColorBlendAttachmentInfo(VkPipelineColorBlendAttachmentState &createInfo){
//         createInfo.blendEnable = VK_FALSE;
//         createInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
//         createInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
//         createInfo.colorBlendOp = VK_BLEND_OP_ADD;
//         createInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//         createInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//         createInfo.alphaBlendOp = VK_BLEND_OP_ADD;
//         createInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//     }
    
// }