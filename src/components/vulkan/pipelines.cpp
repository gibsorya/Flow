#include <pipelines.hpp>
#include <pipeline_infos.hpp>

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
    Error createGraphicsPipeline(vk::Pipeline &pipeline, vk::Device device, vk::PipelineLayout layout, vk::RenderPass renderPass, PipelinePrimitive pPrimitive, vk::Extent2D pSwapExtent,
                                 const PipelineRasterizationState &pRasterizationState, const PipelineMultisampleState &pMultisampleState,
                                 const PipelineDepthStencilState &pDepthStencilState, std::string vertexShaderFile, std::string fragShaderFile)
    {
      auto vertexShaderCode = readFile(vertexShaderFile);
      auto fragShaderCode = readFile(fragShaderFile);

      vk::ShaderModule vertexShader;
      vk::ShaderModule fragmentShader;
      Error err = createShaderModule(vertexShader, vertexShaderCode, device);
      ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create vertex shader module!");
      err = createShaderModule(fragmentShader, fragShaderCode, device);
      ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create fragment shader module!");

      vk::PipelineShaderStageCreateInfo vertexInfo;
      vertexInfo.stage = vk::ShaderStageFlagBits::eVertex;
      vertexInfo.module = vertexShader;
      vertexInfo.pName = "main";

      vk::PipelineShaderStageCreateInfo fragInfo;
      fragInfo.stage = vk::ShaderStageFlagBits::eFragment;
      fragInfo.module = fragmentShader;
      fragInfo.pName = "main";
      vk::PipelineShaderStageCreateInfo stages[] = {vertexInfo, fragInfo};

      auto vertexBindingDescription = Vertex::getBindingDescription();
      auto vertexAttributeDescriptions = Vertex::getAttributeDescription();

      vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
      vertexInputInfo.vertexBindingDescriptionCount = 1;
      vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDescription;
      vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(vertexAttributeDescriptions.size());
      vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();

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

      vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
      inputAssemblyInfo.topology = topologies[pPrimitive];
      inputAssemblyInfo.primitiveRestartEnable = (pPrimitive == PIPELINE_PRIMITIVE_TRIANGLE_STRIPS_WITH_RESTART_INDEX);


      vk::PipelineViewportStateCreateInfo viewportStateInfo;
      viewportStateInfo.viewportCount = 1;
      viewportStateInfo.scissorCount = 1;

      vk::PipelineRasterizationStateCreateInfo rasterizationInfo;

      rasterizationInfo.depthClampEnable = pRasterizationState.enableDepthClamp;
      rasterizationInfo.rasterizerDiscardEnable = pRasterizationState.discardPrimitives;
      rasterizationInfo.polygonMode = (pRasterizationState.wireframeMode) ? vk::PolygonMode::eLine : vk::PolygonMode::eFill;

      local vk::CullModeFlags cullMode[3] = {
          vk::CullModeFlagBits::eNone,
          vk::CullModeFlagBits::eFront,
          vk::CullModeFlagBits::eBack};

      rasterizationInfo.cullMode = cullMode[pRasterizationState.cullMode];
      rasterizationInfo.frontFace = (pRasterizationState.frontFace == FRONT_FACE_CLOCKWISE) ? vk::FrontFace::eClockwise : vk::FrontFace::eCounterClockwise;
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
      std::vector<vk::DynamicState> dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
      };
      dynamicStateInfo.dynamicStateCount = static_cast<u32>(dynamicStates.size());
      dynamicStateInfo.pDynamicStates = dynamicStates.data();

      vk::GraphicsPipelineCreateInfo pipelineInfo;
      pipelineInfo.stageCount = 2;
      pipelineInfo.pStages = stages;
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
      pipelineInfo.pTessellationState = nullptr;
      pipelineInfo.pViewportState = &viewportStateInfo;
      pipelineInfo.pRasterizationState = &rasterizationInfo;
      pipelineInfo.pMultisampleState = &multisamplerInfo;
      // pipelineInfo.pDepthStencilState = &depthStencilInfo;
      pipelineInfo.pColorBlendState = &colorBlendInfo;
      pipelineInfo.pDynamicState = &dynamicStateInfo;
      pipelineInfo.layout = layout;
      pipelineInfo.renderPass = renderPass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
      // pipelineInfo.basePipelineIndex = -1;

      if (device.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }

      device.destroyShaderModule(vertexShader, nullptr);
      device.destroyShaderModule(fragmentShader, nullptr);

      return SUCCESS;
    }

    Error createShaderModule(vk::ShaderModule &shaderModule, const std::vector<char> &code, vk::Device device)
    {
      auto createInfo = vk::ShaderModuleCreateInfo({}, code.size(), reinterpret_cast<const u32 *>(code.data()));

      if (device.createShaderModule(&createInfo, nullptr, &shaderModule) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }

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

    Error createRenderPass(vk::RenderPass &renderPass, vk::Device device, vk::PhysicalDevice physicalDevice, vk::Format swapchainImageFormat)
    {
      // auto colorAttachment =
      //     vk::AttachmentDescription2({},                               /* Flags */
      //                                swapchainImageFormat,             /* Image Format */
      //                                vk::SampleCountFlagBits::e1,      /* Samples */
      //                                vk::AttachmentLoadOp::eClear,     /* Load OP */
      //                                vk::AttachmentStoreOp::eStore,    /* Store OP */
      //                                vk::AttachmentLoadOp::eDontCare,  /* Stencil Load OP */
      //                                vk::AttachmentStoreOp::eDontCare, /* Stencil Store OP */
      //                                vk::ImageLayout::eUndefined,      /* Initial Layout */
      //                                vk::ImageLayout::ePresentSrcKHR); /* Final Layout */
      vk::AttachmentDescription colorAttachment;
      colorAttachment.format = swapchainImageFormat;
      colorAttachment.samples = vk::SampleCountFlagBits::e1;
      colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
      colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
      colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
      colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
      colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
      colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

      auto colorAttachmentRef = vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

      // auto depthAttachment =
      //     vk::AttachmentDescription2({},
      //                                buffers::findDepthFormat(physicalDevice),
      //                                vk::SampleCountFlagBits::e1,
      //                                vk::AttachmentLoadOp::eClear,
      //                                vk::AttachmentStoreOp::eDontCare,
      //                                vk::AttachmentLoadOp::eDontCare,
      //                                vk::AttachmentStoreOp::eDontCare,
      //                                vk::ImageLayout::eUndefined,
      //                                vk::ImageLayout::eDepthStencilAttachmentOptimal);

      // auto depthAttachmentRef = vk::AttachmentReference2(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

      vk::SubpassDescription subpass;
      subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;

      // auto old_subpass =
      //     vk::SubpassDescription2({},                               /* Flags */
      //                             vk::PipelineBindPoint::eGraphics, /* Pipeline Bind Point */
      //                             0,                                /* View Mask */
      //                             0,                                /* Input Attachment Count */
      //                             nullptr,                          /* Input Attachment(s) */
      //                             1,                                /* Color Attachment Count */
      //                             &colorAttachmentRef,              /* Color Attachment(s) */
      //                             nullptr,                          /* Resolve Attachment(s) */
      //                             nullptr,                          /* Depth Attachment(s) */
      //                             0,                                /* Preserve Attachment Count */
      //                             0);                               /* Preserve Attachment(s) */

      auto dependency =
          vk::SubpassDependency(VK_SUBPASS_EXTERNAL,                               /* Src Subpass */
                                0,                                                 /* Dst Subpass */
                                vk::PipelineStageFlagBits::eColorAttachmentOutput, /* Src Stage Mask */
                                vk::PipelineStageFlagBits::eColorAttachmentOutput, /* Dst Stage Mask */
                                {},                                                /* Src Access Mask */
                                vk::AccessFlagBits::eColorAttachmentWrite);        /* Dst Access Mask */

      // std::array<vk::AttachmentDescription2, 1> attachments = {colorAttachment};

      vk::RenderPassCreateInfo renderPassInfo;
      renderPassInfo.attachmentCount = 1;
      renderPassInfo.pAttachments = &colorAttachment;
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpass;
      renderPassInfo.dependencyCount = 1;
      renderPassInfo.pDependencies = &dependency;

      // auto renderPassInfo =
      //     vk::RenderPassCreateInfo2({},               /* Flags */
      //                               1,                /* Attachment Count */
      //                               &colorAttachment, /* Attachments */
      //                               1,                /* Subpass Count */
      //                               &subpass,         /* Subpasses */
      //                               0,                /* Dependency Count */
      //                               nullptr);

      if (device.createRenderPass(&renderPassInfo, nullptr, &renderPass) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }

      return SUCCESS;
    }
  }
}