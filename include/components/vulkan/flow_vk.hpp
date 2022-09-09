#ifndef FLOW_VK
#define FLOW_VK

#include "foundation.hpp"
#include <vulkan/vulkan.hpp>
#include <surfaces.hpp>
#include <instances.hpp>
#include <devices.hpp>
#include <queues.hpp>

struct FlowVkContext {
  FlowVkSurfaces surfaces;
  FlowVkInstances instances;
  FlowVkDevices devices;
};

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext);
}

#endif