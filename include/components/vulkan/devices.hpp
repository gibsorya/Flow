#ifndef FLOW_VK_DEVICES
#define FLOW_VK_DEVICES

#include <foundation.hpp>

struct FlowVkDevices
{
  std::vector<vk::PhysicalDevice> physicalDevices;

  std::vector<vk::Device> devices;
  std::vector<const char *> deviceExtensions; // All extensions for each logical device will be the same for now.
  std::vector<vk::Queue> graphicsQueues;
};

namespace flow::vulkan::devices
{
  Error pickPhysicalDevice(std::vector<vk::PhysicalDevice> &physicalDevices, vk::Instance instance, vk::SurfaceKHR surface);

  bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);

  int rateDeviceSuitability(vk::PhysicalDevice device);

  Error createLogicalDevice(std::vector<vk::Device> &devices, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::Queue &graphicsQueue, vk::Queue &presentQueue);
}

#endif