#pragma once

#include <vulkan/vulkan.h>

#include "platform/window.h"

#include <vector>

namespace engine
{
    void createVulkanInstance(const NativeWindowInfo *native, VkInstance &instance);
    std::vector<const char *> getExtensions(const NativeWindowInfo *native);
    std::vector<const char *> getLayers();
    VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo();
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pMessenger);
    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT messenger,
        const VkAllocationCallbacks *pAllocator);
}