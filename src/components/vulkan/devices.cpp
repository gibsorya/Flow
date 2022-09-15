#include <devices.hpp>
#include <queues.hpp>
#include <swapchains.hpp>

namespace flow::vulkan::devices
{
  Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance, vk::SurfaceKHR surface)
  {
    u32 deviceCount = 0;

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

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapchainAdequate = false;
    if (extensionsSupported)
    {
      SwapchainSupportDetails swapchainSupport = querySwapchainSupport(device, surface);
      swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapchainAdequate;
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

  Error createLogicalDevice(std::vector<vk::Device> &devices, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::Queue &graphicsQueue, vk::Queue &presentQueue, vk::Queue &transferQueue)
  {
    Error err;
    vk::Device device;
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value(), indices.transferFamily.value()};

    float queuePriority = 1.0f;
    for (u32 queueFamily : uniqueQueueFamilies)
    {
      auto createInfo = vk::DeviceQueueCreateInfo({}, queueFamily, 1, &queuePriority);
      queueCreateInfos.push_back(createInfo);
    }

    vk::PhysicalDeviceFeatures2 deviceFeatures;
    physicalDevice.getFeatures2(&deviceFeatures);
    deviceFeatures.features.samplerAnisotropy = VK_TRUE;

    auto createInfo = vk::DeviceCreateInfo({}, static_cast<u32>(queueCreateInfos.size()), queueCreateInfos.data(), 0, nullptr,
                                           static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(), &deviceFeatures.features);

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
    auto transferQueueInfo = vk::DeviceQueueInfo2({}, indices.transferFamily.value(), 0);

    device.getQueue2(&graphicsQueueInfo, &graphicsQueue);
    device.getQueue2(&presentQueueInfo, &presentQueue);
    device.getQueue2(&transferQueueInfo, &transferQueue);

    devices.push_back(device);

    return SUCCESS;
  }

  bool checkDeviceExtensionSupport(vk::PhysicalDevice device)
  {
    u32 extensionCount;

    vk::Result result = device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (result != vk::Result::eSuccess)
    {
      return false;
    }
    std::vector<vk::ExtensionProperties> availableExtensions(extensionCount);
    result = device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions)
    {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }
}