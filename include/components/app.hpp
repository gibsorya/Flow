#ifndef FLOW_APP
#define FLOW_APP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <foundation.hpp>
#include <rendering_device.hpp>
#include <flow_vk.hpp>

global const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

namespace flow
{
  Error setup();
  void mainLoop();
  void cleanup();
  void recreateSwapchain();
  void cleanupSwapchain();
  void draw();
  void framebufferResizeCallback(GLFWwindow *window, int width, int height);
}

#endif