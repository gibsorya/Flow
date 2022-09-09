#include <devices.hpp>
#include <queues.hpp>

namespace flow::vulkan::devices
{
  Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance, vk::SurfaceKHR surface)
  {
    Error err;
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
      if (isDeviceSuitable(physicalDevices.at(0), surface))
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

  bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface)
  {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);

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

  Error createLogicalDevice(std::vector<vk::Device> &devices, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::Queue &graphicsQueue, vk::Queue &presentQueue)
  {
    Error err;
    vk::Device device;
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value()};

    float queuePriority = 1.0f;
    for (u32 queueFamily : uniqueQueueFamilies)
    {
      auto createInfo = vk::DeviceQueueCreateInfo({}, indices.graphicsFamily.value(), 1, &queuePriority);
      queueCreateInfos.push_back(createInfo);
    }

    vk::PhysicalDeviceFeatures2 deviceFeatures;
    physicalDevice.getFeatures2(&deviceFeatures);
    deviceFeatures.features.samplerAnisotropy = VK_TRUE;

    auto createInfo = vk::DeviceCreateInfo({}, static_cast<u32>(queueCreateInfos.size()), queueCreateInfos.data(), 0, nullptr,
                                           0, nullptr, &deviceFeatures.features);

    if (enabledValidationLayers)
    {
      createInfo.enabledLayerCount = static_cast<u32>(layers.size());
      createInfo.ppEnabledLayerNames = layers.data();
    }

    if (physicalDevice.createDevice(&createInfo, nullptr, &device) != vk::Result::eSuccess)
    {
      return ERR_CANT_CREATE;
    }

    auto graphicsQueueInfo = vk::DeviceQueueInfo2({}, indices.graphicsFamily.value(), 0);
    auto presentQueueInfo = vk::DeviceQueueInfo2({}, indices.presentFamily.value(), 0);

    device.getQueue2(&graphicsQueueInfo, &graphicsQueue);
    device.getQueue2(&presentQueueInfo, &presentQueue);

    devices.push_back(device);

    return SUCCESS;
  }
}