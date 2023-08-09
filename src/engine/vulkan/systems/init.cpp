#include "init.hpp"

namespace flow {
  void FlowVkInitializationSystem::Initialize(FlowVkSurfaceComponent &surfaceComponent, FlowVkInstanceComponent &instanceComponent) {
    CreateVkSurface(surfaceComponent);
    CreateVkInstance(instanceComponent, surfaceComponent);
  }

  void FlowVkInitializationSystem::CreateVkSurface(FlowVkSurfaceComponent &surfaceComponent) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    surfaceComponent.window = SDL_CreateWindow(
      "Flow",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      window_flags
    );
  }

  void FlowVkInitializationSystem::CreateVkInstance(FlowVkInstanceComponent &instanceComponent, FlowVkSurfaceComponent &surfaceComponent) {
    VkApplicationInfo appInfo {};
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.applicationVersion = VK_API_VERSION_1_3;
    appInfo.engineVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = "Flow";
    appInfo.pEngineName = "Flow";

    std::vector<const char*> requiredExtensions;
    unsigned int extensionCount = 0;
    const char** extensionNames;

    SDL_Vulkan_GetInstanceExtensions(surfaceComponent.window, &extensionCount, extensionNames);

    for(uint32_t i = 0; i < extensionCount; i++) {
      requiredExtensions.push_back(extensionNames[i]);
    }

    requiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    instanceComponent.createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceComponent.createInfo.flags = 0;
    instanceComponent.createInfo.pApplicationInfo = &appInfo;
    instanceComponent.createInfo.enabledExtensionCount = requiredExtensions.size();
    instanceComponent.createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    instanceComponent.createInfo.enabledLayerCount = 0;

    instanceComponent.createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    VkResult result = vkCreateInstance(&instanceComponent.createInfo, nullptr, &instanceComponent.instance);

    if(result != VK_SUCCESS) {
      throw std::runtime_error("Failed to create Vulkan instance");
    }
  }
}
