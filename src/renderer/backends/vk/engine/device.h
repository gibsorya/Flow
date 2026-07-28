#pragma once

#define VK_VERSION_1_3 1
#include <vulkan/vulkan.h>
#include <vector>

namespace engine
{
    VkPhysicalDevice pickPhysicalGPU(VkInstance instance);
}