#include "pipelines.hpp"
#include "pipelineinfos.hpp"
#include "../buffers.hpp"

namespace flow::vulkan
{
    const vk::SampleCountFlagBits sampleCount[SAMPLES_MAX] = {
        vk::SampleCountFlagBits::e1,
        vk::SampleCountFlagBits::e2,
        vk::SampleCountFlagBits::e4,
        vk::SampleCountFlagBits::e8,
        vk::SampleCountFlagBits::e16,
        vk::SampleCountFlagBits::e32,
        vk::SampleCountFlagBits::e64};

    const vk::CompareOp compareOperators[COMPARE_OP_MAX] = {
        vk::CompareOp::eNever,
        vk::CompareOp::eLess,
        vk::CompareOp::eEqual,
        vk::CompareOp::eLessOrEqual,
        vk::CompareOp::eGreater,
        vk::CompareOp::eNotEqual,
        vk::CompareOp::eGreaterOrEqual,
        vk::CompareOp::eAlways};

    const vk::StencilOp stencilOperators[STENCIL_OP_MAX] = {
        vk::StencilOp::eKeep,
        vk::StencilOp::eZero,
        vk::StencilOp::eReplace,
        vk::StencilOp::eIncrementAndClamp,
        vk::StencilOp::eDecrementAndClamp,
        vk::StencilOp::eInvert,
        vk::StencilOp::eIncrementAndWrap,
        vk::StencilOp::eDecrementAndWrap};

    std::vector<char> readFile(const std::string &fileName)
    {
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    namespace pipelines
    {
        Error createShaderModule(vk::ShaderModule &shaderModule, const std::vector<char> &code, vk::Device device)
        {
            auto createInfo = vk::ShaderModuleCreateInfo({}, code.size(), reinterpret_cast<const u32 *>(code.data()));

            if (device.createShaderModule(&createInfo, nullptr, &shaderModule) != vk::Result::eSuccess)
            {
                return ERR_CANT_CREATE;
            }

            return SUCCESS;
        }

        Error createGraphicsPipeline(vk::Pipeline &pipeline, vk::Device device, vk::PipelineLayout layout, vk::RenderPass renderPass, PipelinePrimitive pPrimitive, vk::Extent2D pSwapExtent, const PipelineRasterizationState &pRasterizationState, const PipelineMultisampleState &pMultisampleState, const PipelineDepthStencilState &pDepthStencilState)
        {
            auto bindingDescription = Vertex::getBindingDescription();
            auto attributeDescriptions = Vertex::getAttributeDescriptions();

            vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;

            local const vk::PrimitiveTopology topologies[PIPELINE_PRIMITIVE_MAX] = {
                vk::PrimitiveTopology::ePointList,
                vk::PrimitiveTopology::eLineList,
                vk::PrimitiveTopology::eLineListWithAdjacency,
                vk::PrimitiveTopology::eLineStrip,
                vk::PrimitiveTopology::eLineStripWithAdjacency,
                vk::PrimitiveTopology::eTriangleList,
                vk::PrimitiveTopology::eTriangleListWithAdjacency,
                vk::PrimitiveTopology::eTriangleStrip,
                vk::PrimitiveTopology::eTriangleStripWithAdjacency,
                vk::PrimitiveTopology::eTriangleStrip,
                vk::PrimitiveTopology::ePatchList};

            inputAssemblyInfo.topology = topologies[pPrimitive];
            inputAssemblyInfo.primitiveRestartEnable = (pPrimitive == PIPELINE_PRIMITIVE_TRIANGLE_STRIPS_WITH_RESTART_INDEX);

            //TODO: Pass the info for this through a parameter
            vk::PipelineTessellationStateCreateInfo tessellationStateInfo;
            // Nothing here yet!

            vk::PipelineViewportStateCreateInfo viewportStateInfo;

            vk::Viewport viewport;
            viewport.width = (float)pSwapExtent.width;
            viewport.height = (float)pSwapExtent.height;
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vk::Rect2D scissor;
            scissor.extent = pSwapExtent;
            scissor.offset = vk::Offset2D(0, 0);

            viewportStateInfo.viewportCount = 1;
            viewportStateInfo.pViewports = &viewport;
            viewportStateInfo.scissorCount = 1;
            viewportStateInfo.pScissors = &scissor;

            vk::PipelineRasterizationStateCreateInfo rasterizationInfo;

            rasterizationInfo.depthClampEnable = pRasterizationState.enableDepthClamp;
            rasterizationInfo.rasterizerDiscardEnable = pRasterizationState.discardPrimitives;
            rasterizationInfo.polygonMode = (pRasterizationState.wireframeMode ? vk::PolygonMode::eLine : vk::PolygonMode::eFill);

            local vk::CullModeFlags cullMode[3] = {
                vk::CullModeFlagBits::eNone,
                vk::CullModeFlagBits::eFront,
                vk::CullModeFlagBits::eBack};

            rasterizationInfo.cullMode = cullMode[pRasterizationState.cullMode];
            rasterizationInfo.frontFace = (pRasterizationState.frontFace == FRONT_FACE_CLOCKWISE ? vk::FrontFace::eClockwise : vk::FrontFace::eCounterClockwise);
            rasterizationInfo.depthBiasEnable = pRasterizationState.enableDepthBias;
            rasterizationInfo.depthBiasConstantFactor = pRasterizationState.depthBiasConstantFactor;
            rasterizationInfo.depthBiasClamp = pRasterizationState.depthBiasClamp;
            rasterizationInfo.depthBiasSlopeFactor = pRasterizationState.depthBiasSlopeFactor;
            rasterizationInfo.lineWidth = pRasterizationState.lineWidth;

            vk::PipelineMultisampleStateCreateInfo multisamplerInfo;

            multisamplerInfo.rasterizationSamples = sampleCount[pMultisampleState.sampleCount];
            multisamplerInfo.sampleShadingEnable = pMultisampleState.enableSampleShading;
            multisamplerInfo.minSampleShading = pMultisampleState.minSampleShading;
            multisamplerInfo.pSampleMask = nullptr; // TODO: Implement this
            multisamplerInfo.alphaToCoverageEnable = pMultisampleState.enableAlphaToCoverage;
            multisamplerInfo.alphaToOneEnable = pMultisampleState.enableAlphaToOne;

            vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;

            depthStencilInfo.depthTestEnable = pDepthStencilState.enableDepthTest;
            depthStencilInfo.depthWriteEnable = pDepthStencilState.enableDepthWrite;
            depthStencilInfo.depthCompareOp = compareOperators[pDepthStencilState.depthCompareOperator];
            depthStencilInfo.depthBoundsTestEnable = pDepthStencilState.enableDepthRange;
            depthStencilInfo.stencilTestEnable = pDepthStencilState.enableStencil;

            depthStencilInfo.front.failOp = stencilOperators[pDepthStencilState.frontOp.fail];
            depthStencilInfo.front.passOp = stencilOperators[pDepthStencilState.frontOp.pass];
            depthStencilInfo.front.depthFailOp = stencilOperators[pDepthStencilState.frontOp.depthFail];
            depthStencilInfo.front.compareOp = compareOperators[pDepthStencilState.frontOp.compare];
            depthStencilInfo.front.compareMask = pDepthStencilState.frontOp.compareMask;
            depthStencilInfo.front.writeMask = pDepthStencilState.frontOp.writeMask;
            depthStencilInfo.front.reference = pDepthStencilState.frontOp.reference;

            depthStencilInfo.back.failOp = stencilOperators[pDepthStencilState.backOp.fail];
            depthStencilInfo.back.passOp = stencilOperators[pDepthStencilState.backOp.pass];
            depthStencilInfo.back.depthFailOp = stencilOperators[pDepthStencilState.backOp.depthFail];
            depthStencilInfo.back.compareOp = compareOperators[pDepthStencilState.backOp.compare];
            depthStencilInfo.back.compareMask = pDepthStencilState.backOp.compareMask;
            depthStencilInfo.back.writeMask = pDepthStencilState.backOp.writeMask;
            depthStencilInfo.back.reference = pDepthStencilState.backOp.reference;

            depthStencilInfo.minDepthBounds = pDepthStencilState.depthRangeMin;
            depthStencilInfo.maxDepthBounds = pDepthStencilState.depthRangeMax;

            vk::PipelineColorBlendAttachmentState colorBlendAttachmentInfo;

            // TODO: Pass the info in as a parameter
            colorBlendAttachmentInfo.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
            colorBlendAttachmentInfo.blendEnable = VK_FALSE;

            vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
            // TODO: Pass the info in as a parameter
            colorBlendInfo.logicOpEnable = VK_FALSE;
            colorBlendInfo.logicOp = vk::LogicOp::eCopy;
            colorBlendInfo.attachmentCount = 1;
            colorBlendInfo.pAttachments = &colorBlendAttachmentInfo;
            colorBlendInfo.blendConstants[0] = 0.0f;
            colorBlendInfo.blendConstants[1] = 0.0f;
            colorBlendInfo.blendConstants[2] = 0.0f;
            colorBlendInfo.blendConstants[3] = 0.0f;

            vk::PipelineDynamicStateCreateInfo dynamicStateInfo;

            std::vector<vk::DynamicState> dynamicStates;

            dynamicStates.push_back(vk::DynamicState::eViewport);
            dynamicStates.push_back(vk::DynamicState::eScissor);

            dynamicStateInfo.dynamicStateCount = static_cast<u32>(dynamicStates.size());
            dynamicStateInfo.pDynamicStates = dynamicStates.data();

            // //TODO: Maybe pass this through a parameter?
            // vk::PipelineLayout layout;
            // Error err = createPipelineLayout(layout, device);
            // ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create pipeline layout!");
            
            //TODO: pass this through a parameter
            auto vertexShaderCode = readFile("data/shaders/mainVert.spv");
            auto fragShaderCode = readFile("data/shaders/mainFrag.spv");
            vk::ShaderModule vertexShader;
            vk::ShaderModule fragmentShader;
            Error err = createShaderModule(vertexShader, vertexShaderCode, device);
            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create vertex shader module!");
            err = createShaderModule(fragmentShader, fragShaderCode, device);
            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create fragment shader module!");
            vk::PipelineShaderStageCreateInfo vertexInfo{};
            vk::PipelineShaderStageCreateInfo fragInfo{};
            populateShaderStageInfo(vertexInfo, vertexShader, vk::ShaderStageFlagBits::eVertex, "main");
            populateShaderStageInfo(fragInfo, fragmentShader, vk::ShaderStageFlagBits::eFragment, "main");
            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = {vertexInfo, fragInfo};

            vk::GraphicsPipelineCreateInfo pipelineInfo;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = shaderStages.data();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
            pipelineInfo.pTessellationState = nullptr;
            pipelineInfo.pViewportState = &viewportStateInfo;
            pipelineInfo.pRasterizationState = &rasterizationInfo;
            pipelineInfo.pMultisampleState = &multisamplerInfo;
            pipelineInfo.pDepthStencilState = nullptr;
            pipelineInfo.pColorBlendState = &colorBlendInfo;
            pipelineInfo.pDynamicState = nullptr;
            pipelineInfo.layout = layout;
            pipelineInfo.renderPass = renderPass;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;

            if (device.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != vk::Result::eSuccess)
            {
                return ERR_CANT_CREATE;
            }

            device.destroyShaderModule(vertexShader, nullptr);
            device.destroyShaderModule(fragmentShader, nullptr);

            return SUCCESS;
        }

        Error createPipelineLayout(vk::PipelineLayout &pipelineLayout, vk::Device device, vk::DescriptorSetLayout setLayout)
        {
            vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &setLayout;
            pipelineLayoutInfo.pushConstantRangeCount = 0;
            pipelineLayoutInfo.pPushConstantRanges = nullptr;

            if (device.createPipelineLayout(&pipelineLayoutInfo, nullptr, &pipelineLayout) != vk::Result::eSuccess)
            {
                return ERR_CANT_CREATE;
            }

            return SUCCESS;
        }

        Error createRenderPass(vk::RenderPass &renderPass, vk::Device device, vk::Format swapchainImageFormat)
        {
            auto colorAttachment =
                vk::AttachmentDescription2({},                               /* Flags */
                                           swapchainImageFormat,             /* Image Format */
                                           vk::SampleCountFlagBits::e1,      /* Samples */
                                           vk::AttachmentLoadOp::eClear,     /* Load OP */
                                           vk::AttachmentStoreOp::eStore,    /* Store OP */
                                           vk::AttachmentLoadOp::eDontCare,  /* Stencil Load OP */
                                           vk::AttachmentStoreOp::eDontCare, /* Stencil Store OP */
                                           vk::ImageLayout::eUndefined,      /* Initial Layout */
                                           vk::ImageLayout::ePresentSrcKHR); /* Final Layout */

            auto colorAttachmentRef = vk::AttachmentReference2(0, vk::ImageLayout::eColorAttachmentOptimal);

            auto subpass = vk::SubpassDescription2({}, vk::PipelineBindPoint::eGraphics, 0, 0, nullptr, 1, &colorAttachmentRef, nullptr, nullptr, 0, 0);

            auto dependency =
                vk::SubpassDependency2(VK_SUBPASS_EXTERNAL,                               /* Src Subpass */
                                       0,                                                 /* Dst Subpass */
                                       vk::PipelineStageFlagBits::eColorAttachmentOutput, /* Src Stage Mask */
                                       vk::PipelineStageFlagBits::eColorAttachmentOutput, /* Dst Stage Mask */
                                       {},                                                /* Src Access Mask */
                                       vk::AccessFlagBits::eColorAttachmentWrite,         /* Dst Access Mask */
                                       {},                                                /* Dependency Flags */
                                       0);                                                /* View Offset */

            auto renderPassInfo =
                vk::RenderPassCreateInfo2({},               /* Flags */
                                          1,                /* Attachment Count */
                                          &colorAttachment, /* Attachments */
                                          1,                /* Subpass Count */
                                          &subpass,         /* Subpasses */
                                          1,                /* Dependency Count */
                                          &dependency);

            if (device.createRenderPass2(&renderPassInfo, nullptr, &renderPass) != vk::Result::eSuccess)
            {
                return ERR_CANT_CREATE;
            }

            // device.createView

            return SUCCESS;
        }

        Error createViewport(vk::Viewport &viewport, vk::Extent2D extent)
        {
            viewport.width = (float)extent.width;
            viewport.height = (float)extent.height;
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            return SUCCESS;
        }

        Error createScissor(vk::Rect2D &scissor, vk::Extent2D extent)
        {
            scissor = vk::Rect2D({0, 0}, extent);

            return SUCCESS;
        }
    }

}

//     VkPipeline createGraphicsPipeline()
//     {
//         VkPipeline pipeline;

//         auto vertexShaderCode = readFile("data/shaders/vert.spv");
//         auto fragShaderCode = readFile("data/shaders/frag.spv");

//         if (root.shaderModules.vertexShaderModules.empty())
//         {
//             root.shaderModules.vertexShaderModules.push_back(createShaderModule(vertexShaderCode));
//             root.shaderModules.fragShaderModules.push_back(createShaderModule(fragShaderCode));
//         }
//         else
//         {
//             root.shaderModules.vertexShaderModules.at(0) = createShaderModule(vertexShaderCode);
//             root.shaderModules.fragShaderModules.at(0) = createShaderModule(fragShaderCode);
//         }

//         VkPipelineShaderStageCreateInfo vertexInfo{};
//         VkPipelineShaderStageCreateInfo fragInfo{};

//         populateShaderStageInfo(vertexInfo, root.shaderModules.vertexShaderModules.at(0), VK_SHADER_STAGE_VERTEX_BIT, "main");
//         populateShaderStageInfo(fragInfo, root.shaderModules.fragShaderModules.at(0), VK_SHADER_STAGE_FRAGMENT_BIT, "main");

//         std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {vertexInfo, fragInfo};

//         if(root.graphicsPipelines.infos.shaderStages.empty()){
//             root.graphicsPipelines.infos.shaderStages.push_back(shaderStages);
//         } else {
//             root.graphicsPipelines.infos.shaderStages.at(0) = shaderStages;
//         }

//         VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
//         VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
//         // VkPipelineTessellationStateCreateInfo tessellation{}; //Implement this later
//         VkPipelineViewportStateCreateInfo viewportInfo{};
//         VkPipelineRasterizationStateCreateInfo rasterization{};
//         VkPipelineMultisampleStateCreateInfo multisampler{};
//         // VkPipelineDepthStencilStateCreateInfo depthStencil{}; //Implement this later
//         VkPipelineColorBlendStateCreateInfo colorBlend{};
//         VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//         // VkPipelineDynamicStateCreateInfo dynamicState{}; //Implement this later

//         if(root.flowViewports.viewports.empty()){
//             root.flowViewports.viewports.push_back(createViewport());
//         } else {
//             root.flowViewports.viewports.at(0) = createViewport();
//         }

//         if(root.flowScissors.scissors.empty()){
//             root.flowScissors.scissors.push_back(createScissor());
//         } else {
//             root.flowScissors.scissors.at(0) = createScissor();
//         }

//         populateVertexInputInfo(vertexInputInfo);
//         populateInputAssemblyInfo(inputAssembly);
//         // populateTesselationInfo(tessellation); //Implement this later
//         populateViewportInfo(viewportInfo);
//         populateRasterizationInfo(rasterization);
//         populateMultisamplerInfo(multisampler);
//         // populateDepthStencilInfo(depthStencil); //Implement this later
//         populateColorBlendAttachmentInfo(colorBlendAttachment);
//         populateColorBlendInfo(colorBlend, colorBlendAttachment);
//         // populateDynamicStateInfo(dynamicState); //Implement this later

//         if (root.graphicsPipelines.pipelineLayouts.empty())
//         {
//             root.graphicsPipelines.pipelineLayouts.push_back(createPipelineLayout());
//         }
//         else
//         {
//             root.graphicsPipelines.pipelineLayouts.at(0) = createPipelineLayout();
//         }

//         VkGraphicsPipelineCreateInfo pipelineInfo{};
//         pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//         pipelineInfo.stageCount = 2;
//         pipelineInfo.pStages = shaderStages.data();
//         pipelineInfo.pVertexInputState = &vertexInputInfo;
//         pipelineInfo.pInputAssemblyState = &inputAssembly;
//         pipelineInfo.pViewportState = &viewportInfo;
//         // pipelineInfo.pTessellationState = &tessellation; //Implement this later
//         pipelineInfo.pRasterizationState = &rasterization;
//         pipelineInfo.pMultisampleState = &multisampler;
//         // pipelineInfo.pDepthStencilState = &depthStencil; //Implement this later
//         pipelineInfo.pColorBlendState = &colorBlend;
//         // pipelineInfo.pDynamicState = &dynamicState; //Implement this later
//         pipelineInfo.layout = root.graphicsPipelines.pipelineLayouts.back();
//         pipelineInfo.renderPass = root.flowRenderPasses.renderPasses.at(0);
//         pipelineInfo.subpass = 0;

//         if (vkCreateGraphicsPipelines(root.flowDevices.devices.at(0), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create graphics pipeline!");
//         }

//         vkDestroyShaderModule(root.flowDevices.devices.at(0), root.shaderModules.vertexShaderModules.at(0), nullptr);
//         vkDestroyShaderModule(root.flowDevices.devices.at(0), root.shaderModules.fragShaderModules.at(0), nullptr);

//         return pipeline;
//     }