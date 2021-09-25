#ifndef FLOW_SURFACE
#define FLOW_SURFACE

#include "../flow/flow.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

typedef struct FlowSurface FlowSurface;
struct FlowSurface{
    vk::SurfaceKHR surface;

    VkBuffer
};

namespace flow::vulkan {
    void createSurface();
}

#endif