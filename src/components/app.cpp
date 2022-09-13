#include <app.hpp>

namespace flow
{
  global FlowVkContext *vkContext = nullptr;

  Error setup()
  {
    Error err = SUCCESS;
    vkContext = new FlowVkContext();

    err = vulkan::initalize(vkContext);
    return err;
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(vkContext->surfaces.window))
    {
      glfwPollEvents();
      draw();
    }

    vkContext->devices.devices.at(0).waitIdle();
  }

  void recreateSwapchain()
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(vkContext->surfaces.window, &width, &height);
    while (width == 0 || height == 0)
    {
      glfwGetFramebufferSize(vkContext->surfaces.window, &width, &height);
      glfwWaitEvents();
    }

    vkContext->devices.devices.at(0).waitIdle();

    cleanupSwapchain();

    Error err = vulkan::swapchains::createSwapchain(vkContext->swaps.swapchains.at(0), vkContext->swaps.swapchainExtents.at(0), vkContext->swaps.swapchainImages, vkContext->swaps.swapchainImageFormats.at(0), vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0), vkContext->surfaces.window);

    if (err != SUCCESS)
    {
      throw std::runtime_error("Failed to recreate swapchain!");
    }

    err = vulkan::swapchains::createImageViews(vkContext->swaps, vkContext->devices.devices.at(0));

    if (err != SUCCESS)
    {
      throw std::runtime_error("Failed to recreate image views!");
    }

    err = vulkan::buffers::createFramebuffers(vkContext->frameBuffers.swapchainFrameBuffers.at(0), vkContext->devices.devices.at(0), vkContext->swaps.swapchainImageViews, vkContext->swaps.swapchainExtents.at(0), vkContext->graphics.renderPasses.at(0));

    if (err != SUCCESS)
    {
      print_error(errors[err], "Failed!");
      throw std::runtime_error("Failed to recreate frame buffers!");
    }
  }

  void cleanupSwapchain()
  {
    for (auto frameBuffer : vkContext->frameBuffers.swapchainFrameBuffers.at(0))
    {
      vkContext->devices.devices.at(0).destroyFramebuffer(frameBuffer);
    }

    for (auto imageView : vkContext->swaps.swapchainImageViews)
    {
      vkContext->devices.devices.at(0).destroyImageView(imageView);
    }

    for (size_t i = 0; i < vkContext->swaps.swapchains.size(); i++)
    {
      vkContext->devices.devices.at(0).destroySwapchainKHR(vkContext->swaps.swapchains.at(i));
    }
  }

  void cleanup()
  {
    cleanupSwapchain();

    for (auto buffer : vkContext->vertexBuffers.vertexBuffers)
    {
      vkContext->devices.devices.at(0).destroyBuffer(buffer);
    }

    for (auto memory : vkContext->vertexBuffers.vertexMemories)
    {
      vkContext->devices.devices.at(0).freeMemory(memory);
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
      vkContext->devices.devices.at(0).destroySemaphore(vkContext->syncObjects.renderFinishedSemaphores[i], nullptr);
      vkContext->devices.devices.at(0).destroySemaphore(vkContext->syncObjects.imageAvailableSemaphores[i], nullptr);
      vkContext->devices.devices.at(0).destroyFence(vkContext->syncObjects.inFlightFences[i], nullptr);
    }

    for (auto commandPool : vkContext->commandPools.commandPools)
    {
      vkContext->devices.devices.at(0).destroyCommandPool(commandPool);
    }

    for (auto pipeline : vkContext->graphics.graphicsPipelines)
    {
      vkContext->devices.devices.at(0).destroyPipeline(pipeline);
    }

    for (auto pipelineLayout : vkContext->graphics.pipelineLayouts)
    {
      vkContext->devices.devices.at(0).destroyPipelineLayout(pipelineLayout);
    }

    for (auto renderPass : vkContext->graphics.renderPasses)
    {
      vkContext->devices.devices.at(0).destroyRenderPass(renderPass);
    }

    for (auto device : vkContext->devices.devices)
    {
      device.destroy();
    }

    if (enabledValidationLayers)
    {
      for (auto debugMessenger : vkContext->instances.debugMessengers)
      {
        vulkan::instances::DestroyDebugUtilsMessengerEXT(vkContext->instances.instances.at(0), debugMessenger, nullptr);
      }
    }

    int i = 0;
    for (auto instance : vkContext->instances.instances)
    {
      instance.destroySurfaceKHR(vkContext->surfaces.surfaces.at(i));
      i++;
    }

    for (auto instance : vkContext->instances.instances)
    {
      instance.destroy();
    }

    glfwDestroyWindow(vkContext->surfaces.window);
    glfwTerminate();
  }

  void draw()
  {
    vk::Result result = vkContext->devices.devices.at(0).waitForFences(1, &vkContext->syncObjects.inFlightFences.at(vkContext->syncObjects.currentFrame), VK_TRUE, UINT64_MAX);

    u32 imageIndex;
    vk::SubmitInfo submitInfo;

    result = vkContext->devices.devices.at(0).acquireNextImageKHR(vkContext->swaps.swapchains.at(0), UINT64_MAX, vkContext->syncObjects.imageAvailableSemaphores.at(vkContext->syncObjects.currentFrame), VK_NULL_HANDLE, &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
      recreateSwapchain();
      return;
    }

    vkContext->devices.devices.at(0).resetFences(1, &vkContext->syncObjects.inFlightFences.at(vkContext->syncObjects.currentFrame));

    vkContext->commandBuffers.commandBuffers.at(0).at(vkContext->syncObjects.currentFrame).reset();
    Error err = vulkan::buffers::recordCommandBuffer(vkContext->commandBuffers.commandBuffers.at(0).at(vkContext->syncObjects.currentFrame), imageIndex, vkContext->graphics.renderPasses.at(0),
                                                     vkContext->swaps.swapchainExtents.at(0), vkContext->frameBuffers.swapchainFrameBuffers.at(0), vkContext->graphics.graphicsPipelines.at(0),
                                                     vkContext->vertexBuffers.vertexBuffers.at(0));
    if (err != SUCCESS)
    {
      throw std::runtime_error("Failed to record command buffer!!");
    }
    std::vector<vk::Semaphore> waitSemaphores = {vkContext->syncObjects.imageAvailableSemaphores.at(vkContext->syncObjects.currentFrame)};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    // vk::SemaphoreSubmitInfo semaphoreSubmitInfo;
    // semaphoreSubmitInfo.semaphore = waitSemaphores.at(0);
    // semaphoreSubmitInfo.stageMask = waitStages;

    // vk::CommandBufferSubmitInfo commandBufferSubmitInfo;
    // commandBufferSubmitInfo.commandBuffer = vkContext->commandBuffers.commandBuffers.at(0);

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkContext->commandBuffers.commandBuffers.at(0).at(vkContext->syncObjects.currentFrame);

    std::vector<vk::Semaphore> signalSemaphores = {vkContext->syncObjects.renderFinishedSemaphores.at(vkContext->syncObjects.currentFrame)};

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    if (vkContext->devices.graphicsQueues.at(0).submit(1, &submitInfo, vkContext->syncObjects.inFlightFences.at(vkContext->syncObjects.currentFrame)) != vk::Result::eSuccess)
    {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores.data();

    vk::SwapchainKHR swapchains[] = {vkContext->swaps.swapchains.at(0)};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;

    presentInfo.pImageIndices = &imageIndex;
    result = vkContext->devices.presentQueues.at(0).presentKHR(&presentInfo);

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || vkContext->syncObjects.framebufferResized)
    {
      vkContext->syncObjects.framebufferResized = false;
      recreateSwapchain();
    }
    else if (result != vk::Result::eSuccess)
    {
      throw std::runtime_error("Failed to present swap chain image!");
    }

    vkContext->syncObjects.currentFrame = (vkContext->syncObjects.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  void framebufferResizeCallback(GLFWwindow *window, int width, int height)
  {
    glfwGetWindowUserPointer(window);
    vkContext->syncObjects.framebufferResized = true;
  }
}