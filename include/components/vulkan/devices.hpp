#ifndef FLOW_VK_DEVICES
#define FLOW_VK_DEVICES

#include <foundation.hpp>

#if __APPLE__
const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#else
const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

struct FlowVkDevices
{
  std::vector<vk::PhysicalDevice> physicalDevices;

  std::vector<vk::Device> devices;
  std::vector<const char *> deviceExtensions; // All extensions for each logical device will be the same for now.
  std::vector<vk::Queue> graphicsQueues;
  std::vector<vk::Queue> presentQueues;
  std::vector<vk::Queue> transferQueues;
};

namespace flow::vulkan::devices
{
  Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance, vk::SurfaceKHR surface);

  bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);

  bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

  int rateDeviceSuitability(vk::PhysicalDevice device);

  Error createLogicalDevice(std::vector<vk::Device> &devices, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::Queue &graphicsQueue, vk::Queue &presentQueue, vk::Queue &transferQueue);
}

#endif