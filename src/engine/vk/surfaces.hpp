#ifndef FLOW_SURFACE
#define FLOW_SURFACE

#include "../flow/flow.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct FlowSurfaces{
    std::vector<VkSurfaceKHR> surfaces;
};

namespace flow::vulkan {
    VkSurfaceKHR createSurface();
}

#endif