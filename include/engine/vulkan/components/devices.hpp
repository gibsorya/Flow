#ifndef FLOW_VULKAN_DEVICES
#define FLOW_VULKAN_DEVICES

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <iostream>
#include <set>
#include "swapchain.hpp"
#include "surface.hpp"

namespace flow {
  struct FlowVkPhysicalDeviceComponent {
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
  };

  struct FlowVkLogicalDeviceComponent {
    VkDevice logicalDevice;

    VkDeviceCreateInfo createInfo;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
  };

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
      return graphicsFamily.has_value();
    }
  };

  const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    #ifdef __APPLE__
    , "VK_KHR_portability_subset"
    #endif
  };

  bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
}

#endif // FLOW_VULKAN_DEVICES
