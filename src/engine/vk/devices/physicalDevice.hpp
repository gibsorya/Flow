#ifndef FLOW_PHYSICAL_DEVICE
#define FLOW_PHYSICAL_DEVICE

#include "../../flow/flow.hpp"

#include <vulkan/vulkan.h>

typedef struct FlowPhysicalDevice FlowPhysicalDevice;
struct FlowPhysicalDevice{
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties2 deviceProperties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    };
    VkPhysicalDeviceFeatures2 deviceFeatures{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2
    };
};
namespace flow::vulkan::devices {
    void pickPhysicalDevices();

    bool isDeviceSuitable(VkPhysicalDevice device);
}

#endif