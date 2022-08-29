#ifndef FLOW_VK
#define FLOW_VK

#include "foundation.hpp"
#include <vulkan/vulkan.hpp>
#include <surfaces.hpp>

struct FlowVkContext {
  FlowVkSurfaces surfaces;
};

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext);
}

#endif