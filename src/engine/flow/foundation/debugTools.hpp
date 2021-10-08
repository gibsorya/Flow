// #ifndef DEBUG_TOOLS
// #define DEBUG_TOOLS

// #include <vulkan/vulkan.h>

// #include <vector>

// namespace flow
// {
// #ifdef NDEBUG
//     const bool enabledValidationLayers = false;
// #else
//     const bool enabledValidationLayers = true;
// #endif
//     namespace validlayers
//     {
//         const std::vector<const char *> layers = {
//             "VK_LAYER_KHRONOS_validation"};
//     }
// }

// struct DebugUtils{
//     VkDebugUtilsMessengerEXT debugMessenger;
// };

// namespace flow::vulkan
// {
//     void setupDebugMessenger();

//     bool checkValidationLayerSupport();

//     std::vector<const char *> getRequiredExtensions();

//     void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

//     VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

//     void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

//     static VkBool32 VKAPI_PTR VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
// }

// #endif