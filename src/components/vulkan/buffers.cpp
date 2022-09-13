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

  Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
  {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
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

  Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue)
  {
    // QueueFamilyIndices indices = findQueueFamilies()
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();

    vk::Result result = device.createBuffer(&bufferInfo, nullptr, &vertexBuffer);

    ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to create vertex buffer!");

    vk::MemoryRequirements memRequirements;
    device.getBufferMemoryRequirements(vertexBuffer, &memRequirements);

    vk::MemoryAllocateInfo memoryInfo;
    memoryInfo.allocationSize = memRequirements.size;
    memoryInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, physicalDevice);

    result = device.allocateMemory(&memoryInfo, nullptr, &vertexBufferMemory);

    ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to create vertex buffer memory!");

    void *data;
    result = device.mapMemory(vertexBufferMemory, 0, bufferInfo.size, {}, &data);
      memcpy(data, vertices.data(), (size_t)bufferInfo.size);
    device.unmapMemory(vertexBufferMemory);

    device.bindBufferMemory(vertexBuffer, vertexBufferMemory, 0);

    return SUCCESS;
  }

  Error recordCommandBuffer(vk::CommandBuffer commandBuffer, u32 imageIndex, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer)
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

    commandBuffer.draw(static_cast<u32>(vertices.size()), 1, 0, 0);

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