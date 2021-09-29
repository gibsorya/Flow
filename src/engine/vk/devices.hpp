#ifndef FLOW_DEVICES
#define FLOW_DEVICES

#include "../flow/flow.hpp"
#include <vulkan/vulkan.h>

typedef struct FlowDevices FlowDevices;
struct FlowDevices {
    std::unique_ptr<std::vector<VkPhysicalDevice>> availablePhysicalDevices = std::make_unique<std::vector<VkPhysicalDevice>>();
    VkPhysicalDevice currentPhysicalDevice;
    VkPhysicalDeviceProperties2 deviceProperties = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    };
    VkPhysicalDeviceFeatures2 deviceFeatures = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
    };


    VkDevice device;
    VkQueue graphicsQueue;
};

namespace flow::vulkan{
    void pickPhysicalDevice(VkInstance* instance);
    bool isDeviceSuitable(VkPhysicalDevice device);

    VkDevice createLogicalDevice(VkPhysicalDevice *physicalDevice);
}

#endif