#ifndef FLOW_PHYSICAL_DEVICE
#define FLOW_PHYSICAL_DEVICE

#include "../../flow/flow.hpp"

#include <vulkan/vulkan.h>

typedef struct FlowPhysicalDevice FlowPhysicalDevice;
struct FlowPhysicalDevice{
    vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE;

    vk::PhysicalDeviceProperties2 deviceProperties;
    vk::PhysicalDeviceFeatures2 deviceFeatures;
};
namespace flow::vulkan::devices {
    void pickPhysicalDevices();

    bool isDeviceSuitable(vk::PhysicalDevice device);
}

#endif