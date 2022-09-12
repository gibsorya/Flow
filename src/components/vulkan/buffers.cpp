#include <buffers.hpp>
#include <queues.hpp>

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

  Error createCommandBuffer(vk::CommandBuffer &commandBuffer, vk::Device device, vk::CommandPool commandPool)
  {
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = 1;

    ERROR_FAIL_COND(device.allocateCommandBuffers(&allocInfo, &commandBuffer) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to allocate command buffer!");

    return SUCCESS;
  }

  Error recordCommandBuffer(vk::CommandBuffer commandBuffer, u32 imageIndex, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Pipeline graphicsPipeline)
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

    commandBuffer.draw(3, 1, 0, 0);

    vk::SubpassEndInfo subpassEndInfo;

    commandBuffer.endRenderPass();

    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
      ERROR_FAIL(FAILED, "Failed to record command buffer!");
    }

    return SUCCESS;
  }
}