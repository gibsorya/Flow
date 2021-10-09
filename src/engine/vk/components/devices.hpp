// #ifndef FLOW_DEVICES
// #define FLOW_DEVICES

// #include "../flow/flow.hpp"
// #include <vulkan/vulkan.h>

// namespace flow::extensions
// {
//     const std::vector<const char *> deviceExtensions = {
//         VK_KHR_SWAPCHAIN_EXTENSION_NAME
//     };

//     const std::vector<const char *> additionalExtensions = {
//         VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
//     };
// }

// struct FlowDevices
// {
//     VkPhysicalDeviceProperties2 deviceProperties = {
//         .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
//     VkPhysicalDeviceFeatures2 deviceFeatures = {
//         .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
//     };

//     VkPhysicalDevice physicalDevice;
//     std::vector<VkDevice> devices;
//     VkQueue graphicsQueue;
//     VkQueue presentQueue;
// };

// namespace flow::vulkan
// {
//     VkPhysicalDevice pickPhysicalDevice();
//     bool isDeviceSuitable(VkPhysicalDevice device);
//     bool checkDeviceExtensionSupport(VkPhysicalDevice device);

//     VkDevice createLogicalDevice();
// }

// #endif