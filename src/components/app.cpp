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
    }
  }

  void cleanup()
  {
    // for (auto imageView : flow->flowDepthBuffers.depthImageViews)
    // {
    //   flow->flowDevices.devices.at(0).destroyImageView(imageView);
    // }
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
}