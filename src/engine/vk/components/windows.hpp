#ifndef FLOW_DISPLAY
#define FLOW_DISPLAY

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

struct FlowSurfaces {
    const u32 WIDTH = 1280;
    const u32 HEIGHT = 720;

    GLFWwindow* window;
    std::vector<vk::SurfaceKHR> surfaces; //In case we need multiple surfaces?
    std::vector<vk::PresentModeKHR> presentModes;
};

#endif