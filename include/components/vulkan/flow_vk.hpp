#ifndef FLOW_VK
#define FLOW_VK

#include "foundation.hpp"
#include <vulkan/vulkan.hpp>
#include <surfaces.hpp>
#include <instances.hpp>
#include <devices.hpp>
#include <queues.hpp>
#include <swapchains.hpp>

struct FlowVkContext {
  FlowVkSurfaces surfaces;
  FlowVkInstances instances;
  FlowVkDevices devices;
  FlowVkSwapchains swaps;
};

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext);
}

#endif