#ifndef FLOW_VULKAN_DEVICES
#define FLOW_VULKAN_DEVICES

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <iostream>
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
    std::vector<const char*> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkQueue graphicsQueue;
    VkQueue presentQueue;
  };

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
      return graphicsFamily.has_value();
    }
  };

  bool isDeviceSuitable(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
}

#endif // FLOW_VULKAN_DEVICES
