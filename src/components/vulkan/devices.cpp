#include <devices.hpp>
#include <queues.hpp>

namespace flow::vulkan::devices
{
  Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance)
  {
    Error err;
    // vk::PhysicalDevice physicalDevice;

    u32 deviceCount;

    vk::Result result = instance.enumeratePhysicalDevices(&deviceCount, nullptr);

    ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_INVALID, "Cannot get device count!");
    ERROR_FAIL_COND(deviceCount == 0, ERR_NOT_FOUND, "Could not find GPU with Vulkan support!");

    physicalDevices.resize((size_t)deviceCount);

    result = instance.enumeratePhysicalDevices(&deviceCount, physicalDevices.data());

    if (deviceCount > 1)
    {
      std::multimap<int, vk::PhysicalDevice> candidates;

      for (const auto &device : physicalDevices)
      {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
      }

      ERROR_FAIL_COND(candidates.rbegin()->first == 0, ERR_NOT_FOUND, "Cannot find suitable GPU!");

      int i = 0;
      for (const auto &candidate : candidates)
      {
        physicalDevices[i] = candidate.second;
        i++;
      }
    }
    else
    {
      if (isDeviceSuitable(physicalDevices.at(0)))
      {
        return SUCCESS;
      }
      else
      {
        return ERR_NOT_FOUND;
      }
    }

    return SUCCESS;
  }

  Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance, vk::SurfaceKHR surface)
  {
  }

  bool isDeviceSuitable(vk::PhysicalDevice device)
  {
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
  }

  int rateDeviceSuitability(vk::PhysicalDevice device)
  {
    int score = 0;

    vk::PhysicalDeviceProperties2 deviceProperties;
    vk::PhysicalDeviceFeatures2 deviceFeatures;

    device.getProperties2(&deviceProperties);
    device.getFeatures2(&deviceFeatures);

    if (deviceProperties.properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
    {
      score += 1000;
    }

    score += deviceProperties.properties.limits.maxImageDimension2D;

    if (!deviceFeatures.features.geometryShader)
    {
      return 0;
    }

    return score;
  }
}