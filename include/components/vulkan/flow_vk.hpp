#ifndef FLOW_VK
#define FLOW_VK

#include "foundation.hpp"
#include <vulkan/vulkan.hpp>
#include <surfaces.hpp>
#include <instances.hpp>
#include <devices.hpp>
#include <queues.hpp>
#include <swapchains.hpp>
#include <pipelines.hpp>
#include <pipeline_infos.hpp>

struct FlowVkContext {
  FlowVkSurfaces surfaces;
  FlowVkInstances instances;
  FlowVkDevices devices;
  FlowVkSwapchains swaps;
  FlowVkGraphics graphics;
};

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext);
}

#endif