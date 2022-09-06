#ifndef FLOW_VK_INSTANCES
#define FLOW_VK_INSTANCES

#include <foundation.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

struct FlowVkInstances
{
  std::vector<vk::Instance> instances;

  std::vector<vk::DebugUtilsMessengerEXT> debugMessengers; // Every instance should have its own debug messenger
  std::vector<const char *> extensions;                    //*This could be stored inside another vector, but for now all extensions will be the same for each potential instance
};

namespace flow::vulkan::instances
{
  Error createInstance(std::vector<vk::Instance> &instances, const char *appName, const char *engineName, std::vector<const char *> extensions);
  void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo);
  Error setupDebugMessenger(vk::Instance instance, std::vector<vk::DebugUtilsMessengerEXT> &debugMessengers);
  std::vector<const char *> findInstanceExtensions();

  VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo, const vk::AllocationCallbacks *pAllocator, vk::DebugUtilsMessengerEXT *pDebugMessenger);
  void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks *pAllocator);
}

#endif