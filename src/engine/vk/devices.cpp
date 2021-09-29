#include "devices.hpp"
#include <engine/root.hpp>

namespace flow::vulkan{
    void pickPhysicalDevice(VkInstance* instance){
        u32 deviceCount;
        vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

        if(deviceCount == 0){
            std::runtime_error("Failed to find GPU with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());
        
        for(const auto& device : devices){
            if(isDeviceSuitable(device)){
                root->flowDevices->currentPhysicalDevice = device;
                break;
            }
        }

        if(root->flowDevices->currentPhysicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
    }

    bool isDeviceSuitable(VkPhysicalDevice device){
        QueueFamilyIndicies indices = findQueueFamilies(device);

        vkGetPhysicalDeviceProperties2(device, &root->flowDevices->deviceProperties);
        vkGetPhysicalDeviceFeatures2(device, &root->flowDevices->deviceFeatures);

        return root->flowDevices->deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
        && root->flowDevices->deviceFeatures.features.geometryShader && indices.isComplete();
    }


    VkDevice createLogicalDevice(VkPhysicalDevice *physicalDevice){
        VkDevice device;
        QueueFamilyIndicies indices = findQueueFamilies(root->flowDevices->currentPhysicalDevice);

        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &root->flowDevices->deviceFeatures.features;
        createInfo.enabledExtensionCount = 0;

        if(root->validLayers->enabledValidationLayers){
            createInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
            createInfo.ppEnabledLayerNames = root->validLayers->layers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(root->flowDevices->currentPhysicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
            throw std::runtime_error("Failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &root->flowDevices->graphicsQueue);

        return device;
    }
}