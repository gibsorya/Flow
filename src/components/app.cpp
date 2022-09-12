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
      try
      {
        draw();
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << std::endl;
        break;
      }
    }

    vkContext->devices.devices.at(0).waitIdle();
  }

  void cleanup()
  {
    // for (auto imageView : flow->flowDepthBuffers.depthImageViews)
    // {
    //   flow->flowDevices.devices.at(0).destroyImageView(imageView);
    // }
    vkContext->devices.devices.at(0).destroySemaphore(vkContext->syncObjects.imageAvailableSemaphores.at(0));
    vkContext->devices.devices.at(0).destroySemaphore(vkContext->syncObjects.renderFinishedSemaphores.at(0));
    vkContext->devices.devices.at(0).destroyFence(vkContext->syncObjects.inFlightFences.at(0));

    for (auto commandPool : vkContext->commandPools.commandPools)
    {
      vkContext->devices.devices.at(0).destroyCommandPool(commandPool);
    }

    for (auto frameBuffer : vkContext->frameBuffers.swapchainFrameBuffers.at(0))
    {
      vkContext->devices.devices.at(0).destroyFramebuffer(frameBuffer);
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

    for (auto imageView : vkContext->swaps.swapchainImageViews)
    {
      vkContext->devices.devices.at(0).destroyImageView(imageView);
    }

    for (size_t i = 0; i < vkContext->swaps.swapchains.size(); i++)
    {
      vkContext->devices.devices.at(0).destroySwapchainKHR(vkContext->swaps.swapchains.at(i));
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
    vk::Result result = vkContext->devices.devices.at(0).waitForFences(1, &vkContext->syncObjects.inFlightFences.at(0), VK_TRUE, UINT64_MAX);
    vkContext->devices.devices.at(0).resetFences(1, &vkContext->syncObjects.inFlightFences.at(0));

    u32 imageIndex;
    vk::SubmitInfo submitInfo;
    vk::AcquireNextImageInfoKHR acquireInfo;
    acquireInfo.swapchain = vkContext->swaps.swapchains.at(0);
    acquireInfo.timeout = UINT64_MAX;
    acquireInfo.semaphore = vkContext->syncObjects.imageAvailableSemaphores.at(0);
    acquireInfo.fence = VK_NULL_HANDLE;
    acquireInfo.deviceMask = 1;

    vkContext->devices.devices.at(0).acquireNextImageKHR(vkContext->swaps.swapchains.at(0), UINT64_MAX, vkContext->syncObjects.imageAvailableSemaphores.at(0), VK_NULL_HANDLE, &imageIndex);

    // vkContext->devices.devices.at(0).acquireNextImage2KHR(&acquireInfo, &imageIndex);

    vkContext->commandBuffers.commandBuffers.at(0).reset();
    Error err = vulkan::buffers::recordCommandBuffer(vkContext->commandBuffers.commandBuffers.at(0), imageIndex, vkContext->graphics.renderPasses.at(0), vkContext->swaps.swapchainExtents.at(0), vkContext->frameBuffers.swapchainFrameBuffers.at(0), vkContext->graphics.graphicsPipelines.at(0));
    if(err != SUCCESS)
    {
      throw std::runtime_error("Failed to record command buffer!!");
    }
    std::vector<vk::Semaphore> waitSemaphores = {vkContext->syncObjects.imageAvailableSemaphores.at(0)};
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
    submitInfo.pCommandBuffers = &vkContext->commandBuffers.commandBuffers.at(0);

    std::vector<vk::Semaphore> signalSemaphores = {vkContext->syncObjects.renderFinishedSemaphores.at(0)};

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    if (vkContext->devices.graphicsQueues.at(0).submit(1, &submitInfo, vkContext->syncObjects.inFlightFences.at(0)) != vk::Result::eSuccess)
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
    vkContext->devices.presentQueues.at(0).presentKHR(&presentInfo);
  }
}