#include "devices.hpp"
#include <engine/root.hpp>

namespace flow::vulkan
{

    VkPhysicalDevice pickPhysicalDevice()
    {
        VkPhysicalDevice physicalDevice;

        u32 deviceCount;
        vkEnumeratePhysicalDevices(root.flowInstances.instances.at(0), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            std::runtime_error("Failed to find GPU with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(root.flowInstances.instances.at(0), &deviceCount, devices.data());

        for (const auto& device : devices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }

        return physicalDevice;
    }

    bool isDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndicies indices = findQueueFamilies(device);

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapchainAdequate = false;
        if (extensionsSupported)
        {
            SwapchainSupportDetails details = querySwapchainSupport(device);
            swapchainAdequate = !details.formats.empty() && !details.presentModes.empty();
        }

        return extensionsSupported && indices.isComplete() && swapchainAdequate;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        u32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(extensions::deviceExtensions.begin(), extensions::deviceExtensions.end());

        for (const auto &extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    VkDevice createLogicalDevice()
    {
        VkDevice device;

        QueueFamilyIndicies indices = findQueueFamilies(root.flowDevices.physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (u32 queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &root.flowDevices.deviceFeatures.features;
        createInfo.enabledExtensionCount = static_cast<u32>(extensions::deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = extensions::deviceExtensions.data();

        if (flow::enabledValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<u32>(flow::validlayers::layers.size());
            createInfo.ppEnabledLayerNames = flow::validlayers::layers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(root.flowDevices.physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &root.flowDevices.graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &root.flowDevices.presentQueue);

        return device;
    }

}