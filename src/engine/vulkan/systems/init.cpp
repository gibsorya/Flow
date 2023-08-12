#include "init.hpp"

namespace flow {
  void FlowVkInitializationSystem::Initialize(FlowVkSurfaceComponent &surfaceComponent, FlowVkInstanceComponent &instanceComponent, FlowVkPhysicalDeviceComponent &physicalDeviceComponent, FlowVkLogicalDeviceComponent &logicalDeviceComponent) {
    CreateVkWindow(surfaceComponent);
    CreateVkInstance(instanceComponent, surfaceComponent);
    CreateVkDebugMessenger(instanceComponent);
    CreateVkSurface(surfaceComponent, instanceComponent);
    PickVkPhysicalDevice(physicalDeviceComponent, instanceComponent);
    CreateVkLogicalDevice(logicalDeviceComponent, physicalDeviceComponent);
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

  void FlowVkInitializationSystem::CreateVkLogicalDevice(FlowVkLogicalDeviceComponent &device, FlowVkPhysicalDeviceComponent &physicalDevice)
  {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice.physicalDevice);
    VkDeviceQueueCreateInfo queueCreateInfo {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    device.createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device.createInfo.queueCreateInfoCount = 1;
    device.createInfo.pQueueCreateInfos = &queueCreateInfo;

    #ifdef __APPLE__
      device.deviceExtensions.push_back("VK_KHR_portability_subset");
    #endif
    device.createInfo.enabledExtensionCount = device.deviceExtensions.size();
    device.createInfo.ppEnabledExtensionNames = device.deviceExtensions.data();

    VkResult result = vkCreateDevice(physicalDevice.physicalDevice, &device.createInfo, nullptr, &device.logicalDevice);

    if(result != VK_SUCCESS) {
      throw std::runtime_error("Failed to create logical device");
    }

    VkDeviceQueueInfo2 queueInfo {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    queueInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueInfo.queueIndex = 0;

    vkGetDeviceQueue2(device.logicalDevice, &queueInfo, &device.graphicsQueue);
  }

  void FlowVkInitializationSystem::PickVkPhysicalDevice(FlowVkPhysicalDeviceComponent &physicalDeviceComponent, FlowVkInstanceComponent &instanceComponent) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instanceComponent.instance, &deviceCount, nullptr);

    if(deviceCount == 0) {
      throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instanceComponent.instance, &deviceCount, devices.data());

    for(const auto& device : devices) {
      if(isDeviceSuitable(device)) {
        physicalDeviceComponent.physicalDevice = device;
        break;
      }
    }

    if(physicalDeviceComponent.physicalDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("Failed to find a suitable GPU");
    }
  }
}
