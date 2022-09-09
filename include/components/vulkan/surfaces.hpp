#ifndef FLOW_VK_SURFACE
#define FLOW_VK_SURFACE

#include <foundation.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

struct FlowVkSurfaces
{
  const u32 WIDTH = 1280;
  const u32 HEIGHT = 720;

  GLFWwindow *window;
  std::vector<vk::SurfaceKHR> surfaces; //In case we need multiple surfaces?
  std::vector<vk::PhysicalDeviceSurfaceInfo2KHR> physicalSurfaceInfo;
};

namespace flow::vulkan::surfaces {
  Error initWindow(GLFWwindow *&window, u32 WIDTH, u32 HEIGHT, const char *title);

  Error createSurface(std::vector<vk::SurfaceKHR> &surfaces, vk::Instance instance, GLFWwindow* window);
}

#endif