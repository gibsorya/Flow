#ifndef LOGICAL_FLOW_DEVICE
#define LOGICAL_FLOW_DEVICE

#include "../../flow/flow.hpp"

#include <vulkan/vulkan.hpp>


typedef struct FlowLogicalDevice FlowLogicalDevice;
struct FlowLogicalDevice {
    vk::Device device;
    vk::Queue graphicsQueue;

    vk::DeviceCreateInfo deviceInfo{};
    vk::DeviceQueueCreateInfo queueCreateInfo{};
};

namespace flow::vulkan::devices {
    void createLogicalDevice();
}

#endif