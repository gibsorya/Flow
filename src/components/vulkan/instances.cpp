#include <instances.hpp>

namespace flow::vulkan::instances
{
  Error createInstance(std::vector<vk::Instance> &instances, const char *appName, const char *engineName, std::vector<const char *> extensions)
  {
    if (enabledValidationLayers && !debug::checkValidationLayerSupport())
    {
      ERROR_FAIL(ERR_INVALID, "Validation Layers enabled, but none are supported!");
    }

    vk::Instance instance;

    auto appInfo = vk::ApplicationInfo(appName, VK_VERSION_1_3, engineName, VK_VERSION_1_3, VK_API_VERSION_1_3);

    auto instanceInfo = vk::InstanceCreateInfo({}, &appInfo, 0, nullptr, static_cast<u32>(extensions.size()), extensions.data());

    #if __APPLE__
      instanceInfo.setFlags(vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR);
    #endif

    vk::DebugUtilsMessengerCreateInfoEXT debugInfo;

    if (enabledValidationLayers)
    {
      instanceInfo.enabledLayerCount = static_cast<u32>(layers.size());
      instanceInfo.ppEnabledLayerNames = layers.data();

      populateDebugMessengerCreateInfo(debugInfo);
      instanceInfo.pNext = (vk::DebugUtilsMessengerCreateInfoEXT *)&debugInfo;
    }

    vk::Result result = vk::createInstance(&instanceInfo, nullptr, &instance);

    if (result != vk::Result::eSuccess)
    {
      return ERR_CANT_CREATE;
    }

    // ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to create Vulkan instance!");
    // return ERR_CANT_CREATE;

    instances.push_back(instance);

    return SUCCESS;
  }

  std::vector<const char *> findInstanceExtensions()
  {
    u32 glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char *> extensions(glfwExtensions, glfwExtensionCount + glfwExtensions);
    extensions.push_back(*glfwExtensions);

    #if __APPLE__
      extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    if (enabledValidationLayers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    return extensions;
  }

  void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo)
  {
    createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
        {},
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        debug::debugCallback,
        nullptr);
  }

  Error setupDebugMessenger(vk::Instance instance, std::vector<vk::DebugUtilsMessengerEXT> &debugMessengers)
  {
    if (!enabledValidationLayers)
      return SUCCESS;

    vk::DebugUtilsMessengerEXT debugMessenger;

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);

    if (result != VK_SUCCESS)
    {
      return ERR_CANT_CREATE;
    }

    // ERROR_FAIL_COND(result != VK_SUCCESS, ERR_CANT_CREATE, "Failed to create debug messenger!");

    debugMessengers.push_back(debugMessenger);

    return SUCCESS;
  }

  VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo, const vk::AllocationCallbacks *pAllocator, vk::DebugUtilsMessengerEXT *pDebugMessenger)
  {
    auto func{reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"))};

    if (func != nullptr)
    {
      return func(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks *>(pAllocator), reinterpret_cast<VkDebugUtilsMessengerEXT *>(pDebugMessenger));
    }
    else
    {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }

  void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks *pAllocator)
  {
    auto func{reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"))};

    if (func != nullptr)
    {
      func(instance, debugMessenger, reinterpret_cast<const VkAllocationCallbacks *>(pAllocator));
    }
  }
}