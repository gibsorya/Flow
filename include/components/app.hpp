#ifndef FLOW_APP
#define FLOW_APP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <foundation.hpp>
#include <flow_vk.hpp>

namespace flow {
  Error setup();
  void mainLoop();
  void cleanup();
  void recreateSwapchain();
  void cleanupSwapchain();
  void draw();
  void framebufferResizeCallback(GLFWwindow *window, int width, int height);
}

#endif