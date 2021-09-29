#ifndef DEBUG_TOOLS
#define DEBUG_TOOLS

#include <vulkan/vulkan.hpp>

#include <vector>

typedef struct ValidLayers ValidLayers;
struct ValidLayers
{
#ifdef NDEBUG
    bool enabledValidationLayers = false;
#else
    bool enabledValidationLayers = true;
#endif
    std::vector<const char *> layers = {
        "VK_LAYER_KHRONOS_validation"};
};

typedef struct DebugUtils DebugUtils;
struct DebugUtils
{
    VkDebugUtilsMessengerEXT debugMessenger;
};

namespace flow::vulkan::debugtools
{
    VkDebugUtilsMessengerEXT setupDebugMessenger(VkInstance *instance);

    bool checkValidationLayerSupport();

    std::vector<const char *> getRequiredExtensions();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

    static VkBool32 VKAPI_PTR VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
}

// namespace flow::vulkan::debugtools
// {
//     bool checkValidationLayerSupport();

//     std::vector<const char *> getRequiredExtensions();

//     static VkBool32 VKAPI_PTR VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

//     void setupDebugMessenger();

//     VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo, const vk::AllocationCallbacks *pAllocator, vk::DebugUtilsMessengerEXT *pDebugMessenger);

//     void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks *pAllocator);
// }

#endif