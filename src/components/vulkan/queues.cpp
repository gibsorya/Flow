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

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
      std::cout << "INDEX: " << i << std::endl;
      if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
      {
        std::cout << "GRAPHICS: " << i << std::endl;
        indices.graphicsFamily = i;
      }

      if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer && !(queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics))
      {
        std::cout << "TRANSFER: " << i << std::endl;
        indices.transferFamily = i;
      }

      VkBool32 presentSupport = false;
      vk::Result result = device.getSurfaceSupportKHR(i, surface, &presentSupport);

      if (presentSupport)
      {
        std::cout << "Present: " << i << std::endl;
        indices.presentFamily = i;
      }

      if (indices.isComplete())
      {
        break;
      }

      i++;
    }

    std::cout << "INDICES: " << indices.graphicsFamily.value() << " | " << indices.presentFamily.value() << " | " << indices.transferFamily.value() << std::endl;

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