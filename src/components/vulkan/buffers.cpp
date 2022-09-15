#include <buffers.hpp>
#include <queues.hpp>
#include <sync_objects.hpp>
#include <app.hpp>

namespace flow::vulkan::buffers
{
  Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass)
  {
    frameBuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
      vk::ImageView attachments[] = {
          swapchainImageViews.at(i)};

      vk::FramebufferCreateInfo createInfo;
      createInfo.renderPass = renderpass;
      createInfo.attachmentCount = 1;
      createInfo.pAttachments = attachments;
      createInfo.width = swapExtent.width;
      createInfo.height = swapExtent.height;
      createInfo.layers = 1;

      if (device.createFramebuffer(&createInfo, nullptr, &frameBuffers[i]) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }
    }

    return SUCCESS;
  }

  Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, u32 queueFamilyIndex)
  {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = {vk::CommandPoolCreateFlagBits::eResetCommandBuffer};

    if (device.createCommandPool(&poolInfo, nullptr, &commandPool) != vk::Result::eSuccess)
    {
      return ERR_CANT_CREATE;
    }

    return SUCCESS;
  }

  Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, vk::Device device, vk::CommandPool commandPool)
  {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = (u32)commandBuffers.size();

    ERROR_FAIL_COND(device.allocateCommandBuffers(&allocInfo, commandBuffers.data()) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to allocate command buffer!");

    return SUCCESS;
  }

  Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue transferQueue, std::array<u32, 2> queueFamilyIndices)
  {
    vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    Error err = createBuffer(stagingBuffer, stagingBufferMemory, device, physicalDevice, bufferSize, vk::SharingMode::eConcurrent, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, queueFamilyIndices);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create staging buffer!");

    void *data;
    vk::Result result = device.mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    device.unmapMemory(stagingBufferMemory);

    err = createBuffer(vertexBuffer, vertexBufferMemory, device, physicalDevice, bufferSize, vk::SharingMode::eConcurrent, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, queueFamilyIndices);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create vertex buffer!");

    copyBuffer(stagingBuffer, vertexBuffer, device, commandPool, bufferSize, transferQueue);

    device.destroyBuffer(stagingBuffer, nullptr);
    device.freeMemory(stagingBufferMemory, nullptr);

    return SUCCESS;
  }

  Error createIndexBuffer(vk::Buffer &indexBuffer, vk::DeviceMemory &indexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue transferQueue, std::array<u32, 2> queueFamilyIndices)
  {
    vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    Error err = createBuffer(stagingBuffer, stagingBufferMemory, device, physicalDevice, bufferSize, vk::SharingMode::eConcurrent, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, queueFamilyIndices);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create staging buffer!");

    void *data;
    vk::Result result = device.mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    device.unmapMemory(stagingBufferMemory);

    err = createBuffer(indexBuffer, indexBufferMemory, device, physicalDevice, bufferSize, vk::SharingMode::eConcurrent, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, queueFamilyIndices);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create index buffer!");

    copyBuffer(stagingBuffer, indexBuffer, device, commandPool, bufferSize, transferQueue);

    device.destroyBuffer(stagingBuffer, nullptr);
    device.freeMemory(stagingBufferMemory, nullptr);

    return SUCCESS;
  }

  Error createBuffer(vk::Buffer &buffer, vk::DeviceMemory &bufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size, vk::SharingMode sharingMode, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, std::array<u32, 2> queueFamilyIndices)
  {
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.size = size;
    bufferInfo.sharingMode = sharingMode;
    bufferInfo.usage = usage;
    bufferInfo.queueFamilyIndexCount = queueFamilyIndices.size();
    bufferInfo.pQueueFamilyIndices = queueFamilyIndices.data();

    vk::Result result = device.createBuffer(&bufferInfo, nullptr, &buffer);

    ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to create buffer!");

    vk::MemoryRequirements memRequirements;
    device.getBufferMemoryRequirements(buffer, &memRequirements);

    vk::MemoryAllocateInfo memoryInfo;
    memoryInfo.allocationSize = memRequirements.size;
    memoryInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, physicalDevice);

    result = device.allocateMemory(&memoryInfo, nullptr, &bufferMemory);

    device.bindBufferMemory(buffer, bufferMemory, 0);

    return SUCCESS;
  }

  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::Device device, vk::CommandPool commandPool, vk::DeviceSize size, vk::Queue transferQueue)
  {
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;

    vk::CommandBuffer commandBuffer;
    device.allocateCommandBuffers(&allocInfo, &commandBuffer);

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    commandBuffer.begin(&beginInfo);

    vk::BufferCopy bufferCopy;
    bufferCopy.size = size;
    bufferCopy.srcOffset = 0;
    bufferCopy.dstOffset = 0;
    commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &bufferCopy);

    commandBuffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    transferQueue.submit(1, &submitInfo, VK_NULL_HANDLE);
    transferQueue.waitIdle();

    device.freeCommandBuffers(commandPool, 1, &commandBuffer);
  }

  Error recordCommandBuffer(vk::CommandBuffer commandBuffer, u32 imageIndex, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, vk::Buffer indexBuffer)
  {
    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = {};
    beginInfo.pInheritanceInfo = nullptr;

    ERROR_FAIL_COND(commandBuffer.begin(&beginInfo) != vk::Result::eSuccess, FAILED, "Failed to begin recording command buffer!");

    vk::ClearValue clearColor;
    clearColor.color.float32 = {{0.0f, 0.0f, 0.0f, 1.0f}};

    vk::RenderPassBeginInfo renderPassInfo;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapchainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = extent;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    vk::Buffer vertexBuffers[] = {vertexBuffer};
    vk::DeviceSize offsets[] = {0};

    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    commandBuffer.setViewport(0, 1, &viewport);

    vk::Rect2D scissor;
    scissor.offset = {{0, 0}};
    scissor.extent = extent;
    commandBuffer.setScissor(0, 1, &scissor);

    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

    commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16);

    commandBuffer.drawIndexed(static_cast<u32>(indices.size()), 1, 0, 0, 0);

    vk::SubpassEndInfo subpassEndInfo;

    commandBuffer.endRenderPass();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
      ERROR_FAIL(FAILED, "Failed to record command buffer!");
    }

    return SUCCESS;
  }

  u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice)
  {
    vk::PhysicalDeviceMemoryProperties2 memProperties;
    physicalDevice.getMemoryProperties2(&memProperties);

    for (u32 i = 0; i < memProperties.memoryProperties.memoryTypeCount; i++)
    {
      if ((typeFilter & (1 << i)) && (memProperties.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
      {
        return i;
      }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
  }
}