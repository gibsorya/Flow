#include <queues.hpp>

namespace flow::vulkan
{
  QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface)
  {
    QueueFamilyIndices indices;

    u32 queueFamilyCount = 0;
    device.getQueueFamilyProperties2(&queueFamilyCount, nullptr);

    std::vector<vk::QueueFamilyProperties2> queueFamilies(queueFamilyCount);
    device.getQueueFamilyProperties2(&queueFamilyCount, queueFamilies.data());

// In MacOS, there is no queue count or queue family count on Metal devices,
// so one could create 1 or any number of queues and get full utilization of the
// device.
#if defined(__APPLE__)
    indices.transferFamily = 0;
#endif

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
      if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
      {
        indices.graphicsFamily = i;
      }

      if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer && !(queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics))
      {
        indices.transferFamily = i;
      }

      VkBool32 presentSupport = false;
      vk::Result result = device.getSurfaceSupportKHR(i, surface, &presentSupport);

      if (presentSupport)
      {
        indices.presentFamily = i;
      }

      if (indices.isComplete())
      {
        break;
      }

      i++;
    }

    return indices;
  }

  void printQueueFamilies(vk::PhysicalDevice device)
  {
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    for (auto &queueFamily : queueFamilies)
    {
      std::cout << "Queue number: " + std::to_string(queueFamily.queueCount) << std::endl;
      std::cout << "Queue flags: " + to_string(queueFamily.queueFlags) << std::endl;
    }
  }
}