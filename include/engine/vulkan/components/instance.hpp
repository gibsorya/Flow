#ifndef FLOW_VULKAN_INSTANCE
#define FLOW_VULKAN_INSTANCE

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <SDL_vulkan.h>

namespace flow {
  const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
  };

  #ifdef NDEBUG
    const bool enableValidationLayers = false;
  #else
    const bool enableValidationLayers = true;
  #endif

  struct FlowVkInstanceComponent {
    VkInstance instance;
    VkInstanceCreateInfo createInfo;
    VkDebugUtilsMessengerEXT debugMessenger;
  };

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

  bool checkValidationLayerSupport();

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

  std::vector<const char*> getRequiredExtensions(SDL_Window* window);

  VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

  void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
}

#endif // FLOW_VULKAN_INSTANCE
