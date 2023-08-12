#include "devices.hpp"

namespace flow {
  bool isDeviceSuitable(VkPhysicalDevice device)
  {
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
  }

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
  {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
    // Set the STYPE for each queue family
    for(auto& queueFamily : queueFamilies) {
      queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
    }

    vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for(auto& queueFamily : queueFamilies) {
      if(queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphicsFamily = i;
      }

      if(indices.isComplete()) {
        break;
      }

      i++;
    }

    return indices;
  }
}

