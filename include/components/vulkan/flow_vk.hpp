#ifndef FLOW_VK
#define FLOW_VK

#include "foundation.hpp"
#include <vulkan/vulkan.hpp>
#include <surfaces.hpp>
#include <instances.hpp>

struct FlowVkContext {
  FlowVkSurfaces surfaces;
  FlowVkInstances instances;
};

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext);
}

#endif