#include "renderer/backends/vk/engine/device.h"
#include "device.h"
#include "renderer/backends/vk/helpers.h"

#include <map>

int rateDeviceSuitability(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties2 deviceProperties;
    deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    VkPhysicalDeviceFeatures2 deviceFeatures;
    deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

    vkGetPhysicalDeviceProperties2(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    int score = 0;

    if(deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
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

VkPhysicalDevice engine::pickPhysicalGPU(VkInstance instance)
{
    uint32_t deviceCount{ 0 };
    checkVkResult(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr), "Find number of physical devices that support Vulkan");
    
    if(deviceCount == 0) {
        fprintf(stderr, "No devices on this computer support Vulkan.\n");
        exit(EXIT_FAILURE);
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    checkVkResult(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()), "Could not collect physical devices.");

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& gpu : devices) {
        int score = rateDeviceSuitability(gpu);
        candidates.insert(std::make_pair(score, gpu));    
    }

    if(candidates.rbegin()->first > 0)
    {
        return candidates.rbegin()->second;
    } else {
        fprintf(stderr, "No GPU is suitable for this engine.");
        exit(EXIT_FAILURE);
        return VK_NULL_HANDLE;
    }
}
