#ifndef FLOW_DEVICES
#define FLOW_DEVICES

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>

// namespace flow::extensions
// {
const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

//     const std::vector<const char *> additionalExtensions = {
//         VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
//     };
// }

struct FlowDevices
{
    std::vector<vk::PhysicalDevice> physicalDevices;
    std::vector<vk::PhysicalDeviceProperties2> physicalDeviceProperties;
    std::vector<vk::PhysicalDeviceFeatures2> physicalDeviceFeatures;

    std::vector<vk::Device> devices;
    std::vector<const char *> deviceExtensions; //All extensions for each logical device will be the same for now.
    std::vector<vk::Queue> graphicsQueues;
    std::vector<vk::Queue> presentQueues;
};

namespace flow::vulkan::devices
{
    Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance, vk::SurfaceKHR surface);
    bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);
    bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
    int rateDeviceSuitability(vk::PhysicalDevice device);

    Error createLogicalDevice(std::vector<vk::Device> &devices, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::Queue &graphicsQueue, vk::Queue &presentQueue);
}

#endif