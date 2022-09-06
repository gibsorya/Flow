#ifndef DEBUG_TOOLS
#define DEBUG_TOOLS

#include <vulkan/vulkan.hpp>
#include <foundation.hpp>
#include <iostream>
#include <vector>

#ifdef NDEBUG
const bool enabledValidationLayers = false;
#else
const bool enabledValidationLayers = true;
#endif

const std::vector<const char *> layers = {
    "VK_LAYER_KHRONOS_validation"
};

namespace flow::debug
{
    bool checkValidationLayerSupport();

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                 void *pUserData);
}

#endif