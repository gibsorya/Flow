#ifndef FLOW_CONTEXT
#define FLOW_CONTEXT

#include <flow/foundation.hpp>
#include <vulkan/vulkan.h>

struct FlowContext{
    std::vector<VkInstance> instances;
};

namespace flow::vulkan {
    Error initialize(FlowContext *flow);
}

#endif