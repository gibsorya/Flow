#pragma once

#define VK_VERSION_1_3 1
#include <vulkan/vulkan.h>
#include <vector>

struct QueueFamilyIndices
{
    uint32_t graphics = UINT32_MAX;
    uint32_t present = UINT32_MAX;
    // uint32_t compute = UINT32_MAX;
    // uint32_t transfer = UINT32_MAX;

    bool isComplete() const {
        return graphics != UINT32_MAX && present != UINT32_MAX;
    }
};

namespace engine
{
    VkPhysicalDevice pickPhysicalGPU(VkInstance instance);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice gpu, VkSurfaceKHR surface);
    void setQueueCreateInfo(uint32_t index, std::vector<float> &priorities, VkDeviceQueueCreateInfo &info);
    void createLogicalDevice(VkPhysicalDevice gpu, VkDeviceQueueCreateInfo info, VkDevice &device);
}