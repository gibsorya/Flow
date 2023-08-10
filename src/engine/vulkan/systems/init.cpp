#include "init.hpp"

namespace flow {
  void FlowVkInitializationSystem::Initialize(FlowVkSurfaceComponent &surfaceComponent, FlowVkInstanceComponent &instanceComponent) {
    CreateVkWindow(surfaceComponent);
    CreateVkInstance(instanceComponent, surfaceComponent);
    CreateVkDebugMessenger(instanceComponent);
    CreateVkSurface(surfaceComponent, instanceComponent);
  }

  void FlowVkInitializationSystem::CreateVkWindow(FlowVkSurfaceComponent &surfaceComponent) {
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
    if (enableValidationLayers && !checkValidationLayerSupport()) {
      throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo {};
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.applicationVersion = VK_API_VERSION_1_3;
    appInfo.engineVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = "Flow";
    appInfo.pEngineName = "Flow";

    auto extensions = getRequiredExtensions(surfaceComponent.window);

    instanceComponent.createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceComponent.createInfo.flags = 0;
    instanceComponent.createInfo.pApplicationInfo = &appInfo;
    instanceComponent.createInfo.enabledExtensionCount = extensions.size();
    instanceComponent.createInfo.ppEnabledExtensionNames = extensions.data();
    instanceComponent.createInfo.enabledLayerCount = 0;

    if(enableValidationLayers) {
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      populateDebugMessengerCreateInfo(debugCreateInfo);
      instanceComponent.createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
      instanceComponent.createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      instanceComponent.createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    #ifdef __APPLE__
      instanceComponent.createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    VkResult result = vkCreateInstance(&instanceComponent.createInfo, nullptr, &instanceComponent.instance);

    if(result != VK_SUCCESS) {
      throw std::runtime_error("Failed to create Vulkan instance");
    }
  }

  void FlowVkInitializationSystem::CreateVkSurface(FlowVkSurfaceComponent &surfaceComponent, FlowVkInstanceComponent &instanceComponent) {
    SDL_bool result = SDL_Vulkan_CreateSurface(surfaceComponent.window, instanceComponent.instance, &surfaceComponent.surface);

    if(result != SDL_TRUE) {
      auto error = SDL_GetError();
      throw std::runtime_error("Failed to create Vulkan surface: " + std::string(error));
    }
  }

  void FlowVkInitializationSystem::CreateVkDebugMessenger(FlowVkInstanceComponent &instanceComponent) {
    if(!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if(CreateDebugUtilsMessengerEXT(instanceComponent.instance, &createInfo, nullptr, &instanceComponent.debugMessenger) != VK_SUCCESS) {
      throw std::runtime_error("Failed to set up debug messenger");
    }
  }
}
