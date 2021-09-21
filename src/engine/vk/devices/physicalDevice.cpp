#include "physicalDevice.hpp"
#include "../../root.hpp"

namespace flow::vulkan::devices {
    void pickPhysicalDevices(){
        u32 deviceCount;
        vkEnumeratePhysicalDevices(root->flowInstance->instance, &deviceCount, nullptr);

        if(deviceCount == 0){
            std::runtime_error("Failed to find GPU with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(root->flowInstance->instance, &deviceCount, devices.data());

        for(const auto& device : devices){
            if(isDeviceSuitable(device)){
                root->flowPDs->physicalDevice = device;
                break;
            }
        }

        if(root->flowPDs->physicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
    }

    bool isDeviceSuitable(VkPhysicalDevice device){
        QueueFamilyIndicies indices = findQueueFamilies(device);

        vkGetPhysicalDeviceProperties2(device, &root->flowPDs->deviceProperties);
        vkGetPhysicalDeviceFeatures2(device, &root->flowPDs->deviceFeatures);
        return root->flowPDs->deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && root->flowPDs->deviceFeatures.features.geometryShader && indices.isComplete();
    }
}