#include "devices.hpp"
#include <engine/vk/flow_context.hpp>

namespace flow::vulkan::devices
{
    Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance)
    {
        Error err;
        vk::PhysicalDevice physicalDevice;

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
            if(isDeviceSuitable(physicalDevices.at(0))){
                return SUCCESS;
            } else {
                return ERR_NOT_FOUND;
            }
        }

        return SUCCESS;
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

    bool isDeviceSuitable(vk::PhysicalDevice device)
    {
        QueueFamilyIndices indices = findQueueFamilies(device);

        return indices.isComplete();
    }

}

// namespace flow::vulkan
// {

//     VkPhysicalDevice pickPhysicalDevice()
//     {
//         VkPhysicalDevice physicalDevice;

//         u32 deviceCount;
//         vkEnumeratePhysicalDevices(root.flowInstances.instances.at(0), &deviceCount, nullptr);

//         if (deviceCount == 0)
//         {
//             std::runtime_error("Failed to find GPU with Vulkan support!");
//         }

//         std::vector<VkPhysicalDevice> devices(deviceCount);
//         vkEnumeratePhysicalDevices(root.flowInstances.instances.at(0), &deviceCount, devices.data());

//         for (const auto& device : devices)
//         {
//             if (isDeviceSuitable(device))
//             {
//                 physicalDevice = device;
//                 break;
//             }
//         }

//         if (physicalDevice == VK_NULL_HANDLE)
//         {
//             throw std::runtime_error("Failed to find a suitable GPU!");
//         }

//         return physicalDevice;
//     }

//     bool isDeviceSuitable(VkPhysicalDevice device)
//     {
//         QueueFamilyIndicies indices = findQueueFamilies(device);

//         bool extensionsSupported = checkDeviceExtensionSupport(device);

//         bool swapchainAdequate = false;
//         if (extensionsSupported)
//         {
//             SwapchainSupportDetails details = querySwapchainSupport(device);
//             swapchainAdequate = !details.formats.empty() && !details.presentModes.empty();
//         }

//         return extensionsSupported && indices.isComplete() && swapchainAdequate;
//     }

//     bool checkDeviceExtensionSupport(VkPhysicalDevice device)
//     {
//         u32 extensionCount;
//         vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

//         std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//         vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

//         std::set<std::string> requiredExtensions(extensions::deviceExtensions.begin(), extensions::deviceExtensions.end());

//         for (const auto &extension : availableExtensions)
//         {
//             requiredExtensions.erase(extension.extensionName);
//         }

//         return requiredExtensions.empty();
//     }

//     VkDevice createLogicalDevice()
//     {
//         VkDevice device;

//         QueueFamilyIndicies indices = findQueueFamilies(root.flowDevices.physicalDevice);

//         std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//         std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

//         float queuePriority = 1.0f;
//         for (u32 queueFamily : uniqueQueueFamilies)
//         {
//             VkDeviceQueueCreateInfo queueCreateInfo{};
//             queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//             queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
//             queueCreateInfo.queueCount = 1;
//             queueCreateInfo.pQueuePriorities = &queuePriority;
//             queueCreateInfos.push_back(queueCreateInfo);
//         }

//         // std::vector<const char *> extensions;
//         // extensions.reserve(extensions::deviceExtensions.size() + extensions::additionalExtensions.size());
//         // extensions.insert(extensions.end(), extensions::deviceExtensions.begin(), extensions::deviceExtensions.end());
//         // extensions.insert(extensions.end(), extensions::additionalExtensions.begin(), extensions::additionalExtensions.end());

//         VkDeviceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//         createInfo.pQueueCreateInfos = queueCreateInfos.data();
//         createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
//         createInfo.pEnabledFeatures = &root.flowDevices.deviceFeatures.features;
//         createInfo.enabledExtensionCount = static_cast<u32>(extensions::deviceExtensions.size());
//         createInfo.ppEnabledExtensionNames = extensions::deviceExtensions.data();

//         if (flow::enabledValidationLayers)
//         {
//             createInfo.enabledLayerCount = static_cast<u32>(flow::validlayers::layers.size());
//             createInfo.ppEnabledLayerNames = flow::validlayers::layers.data();
//         }
//         else
//         {
//             createInfo.enabledLayerCount = 0;
//         }

//         if (vkCreateDevice(root.flowDevices.physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create logical device!");
//         }

//         vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &root.flowDevices.graphicsQueue);
//         vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &root.flowDevices.presentQueue);

//         return device;
//     }

// }