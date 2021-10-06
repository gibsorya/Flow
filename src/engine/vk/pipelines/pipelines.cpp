#include "pipelines.hpp"
#include "../../root.hpp"
namespace flow::vulkan::pipelines
{

    local std::vector<char> readFile(const std::string &fileName)
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

    VkShaderModule createShaderModule(const std::vector<char> &code)
    {
        VkShaderModule shaderModule;

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const u32 *>(code.data());

        if (vkCreateShaderModule(root.flowDevices.devices.at(0), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module!");
        }

        return shaderModule;
    }

    VkPipeline createGraphicsPipeline()
    {
        VkPipeline pipeline;

        auto vertexShaderCode = readFile("data/shaders/vert.spv");
        auto fragShaderCode = readFile("data/shaders/frag.spv");

        root.shaderModules.vertexShaderModules.push_back(createShaderModule(vertexShaderCode));
        root.shaderModules.fragShaderModules.push_back(createShaderModule(fragShaderCode));

        VkPipelineShaderStageCreateInfo vertexInfo{};
        VkPipelineShaderStageCreateInfo fragInfo{};

        populateShaderStageInfo(vertexInfo, root.shaderModules.vertexShaderModules.at(0), VK_SHADER_STAGE_VERTEX_BIT, "main");
        populateShaderStageInfo(fragInfo, root.shaderModules.fragShaderModules.at(0), VK_SHADER_STAGE_FRAGMENT_BIT, "main");

        std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {vertexInfo, fragInfo};
        root.graphicsPipelines.infos.shaderStages.push_back(shaderStages);

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        // VkPipelineTessellationStateCreateInfo tessellation{}; //Implement this later
        VkPipelineViewportStateCreateInfo viewportInfo{};
        VkPipelineRasterizationStateCreateInfo rasterization{};
        VkPipelineMultisampleStateCreateInfo multisampler{};
        // VkPipelineDepthStencilStateCreateInfo depthStencil{}; //Implement this later
        VkPipelineColorBlendStateCreateInfo colorBlend{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        // VkPipelineDynamicStateCreateInfo dynamicState{}; //Implement this later
        root.flowViewports.viewports.push_back(createViewport());
        root.flowScissors.scissors.push_back(createScissor());

        populateVertexInputInfo(vertexInputInfo);
        populateInputAssemblyInfo(inputAssembly);
        // populateTesselationInfo(tessellation); //Implement this later
        populateViewportInfo(viewportInfo);
        populateRasterizationInfo(rasterization);
        populateMultisamplerInfo(multisampler);
        // populateDepthStencilInfo(depthStencil); //Implement this later
        populateColorBlendAttachmentInfo(colorBlendAttachment);
        populateColorBlendInfo(colorBlend, colorBlendAttachment);
        // populateDynamicStateInfo(dynamicState); //Implement this later

        root.graphicsPipelines.pipelineLayouts.push_back(createPipelineLayout());

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportInfo;
        // pipelineInfo.pTessellationState = &tessellation; //Implement this later
        pipelineInfo.pRasterizationState = &rasterization;
        pipelineInfo.pMultisampleState = &multisampler;
        // pipelineInfo.pDepthStencilState = &depthStencil; //Implement this later
        pipelineInfo.pColorBlendState = &colorBlend;
        // pipelineInfo.pDynamicState = &dynamicState; //Implement this later
        pipelineInfo.layout = root.graphicsPipelines.pipelineLayouts.back();
        pipelineInfo.renderPass = root.flowRenderPasses.renderPasses.at(0);
        pipelineInfo.subpass = 0;

        if(vkCreateGraphicsPipelines(root.flowDevices.devices.at(0), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS){
            throw std::runtime_error("Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(root.flowDevices.devices.at(0), root.shaderModules.vertexShaderModules.at(0), nullptr);
        vkDestroyShaderModule(root.flowDevices.devices.at(0), root.shaderModules.fragShaderModules.at(0), nullptr);

        return pipeline;
    }

    VkViewport createViewport() {
        VkViewport viewport;

        viewport.width = (float)root.flowSwaps.swapchainExtents.at(0).width;
        viewport.height = (float)root.flowSwaps.swapchainExtents.at(0).height;
        viewport.x = 0;
        viewport.y = 0;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        
        return viewport;
    }

    VkRect2D createScissor() {
        VkRect2D scissor;
        
        scissor.extent = root.flowSwaps.swapchainExtents.at(0);
        scissor.offset = {0, 0};

        return scissor;
    }

    VkPipelineLayout createPipelineLayout() {
        VkPipelineLayout pipelineLayout;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(root.flowDevices.devices.at(0), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }

        return pipelineLayout;    
    }

}