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

QueueFamilyIndices engine::findQueueFamilies(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t count{0};
    vkGetPhysicalDeviceQueueFamilyProperties2(gpu, &count, nullptr);
    std::vector<VkQueueFamilyProperties2> families(count);
    for (auto &family : families)
    {
        family.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        family.pNext = nullptr;
    }
    vkGetPhysicalDeviceQueueFamilyProperties2(gpu, &count, families.data());

    for (uint32_t i = 0; i < count; ++i)
    {
        if(families[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &presentSupport);
        if(presentSupport) indices.present = i;

        if(indices.isComplete()) break;
    }

    return indices;
}

void engine::setQueueCreateInfo(uint32_t index, std::vector<float> &priorities, VkDeviceQueueCreateInfo &info)
{
    info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    info.queueFamilyIndex = index;
    info.queueCount = static_cast<uint32_t>(priorities.size());
    info.pQueuePriorities = priorities.data();
}

void engine::createLogicalDevice(VkPhysicalDevice gpu, VkDeviceQueueCreateInfo info, VkDevice &device)
{
    const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceVulkan12Features enabledVk12Features{};
    enabledVk12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    enabledVk12Features.descriptorIndexing = true;
    enabledVk12Features.shaderSampledImageArrayNonUniformIndexing = true;
    enabledVk12Features.descriptorBindingVariableDescriptorCount = true;
    enabledVk12Features.runtimeDescriptorArray = true;
    enabledVk12Features.bufferDeviceAddress = true;

    VkPhysicalDeviceVulkan13Features enabledVk13Features{};
    enabledVk13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    enabledVk13Features.pNext = &enabledVk12Features;
    enabledVk13Features.synchronization2 = true;
    enabledVk13Features.dynamicRendering = true;

    VkPhysicalDeviceFeatures2 deviceFeatures{};
    deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    deviceFeatures.features.samplerAnisotropy = true;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = &enabledVk13Features;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &info;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures.features;

    checkVkResult(vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &device), "Creating Vulkan logical device");
}
