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

    for (auto instance : vkContext->instances.instances)
    {
      instance.destroy();
    }

    glfwDestroyWindow(vkContext->surfaces.window);
    glfwTerminate();
  }
}