#include <queues.hpp>

namespace flow::vulkan
{
  QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device)
  {
    QueueFamilyIndices indices;

    u32 queueFamilyCount = 0;
    device.getQueueFamilyProperties2(&queueFamilyCount, nullptr);

    std::vector<vk::QueueFamilyProperties2> queueFamilies(queueFamilyCount);
    device.getQueueFamilyProperties2(&queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
      if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
      {
        indices.graphicsFamily = i;
      }

      // VkBool32 presentSupport = false;
      // vk::Result result = device.getSurfaceSupportKHR(i, surface, &presentSupport);

      // if (presentSupport)
      // {
      //   indices.presentFamily = i;
      // }

      if (indices.isComplete())
      {
        break;
      }

      i++;
    }

    return indices;
  }
}