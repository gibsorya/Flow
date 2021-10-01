#ifndef FLOW_INSTANCES
#define FLOW_INSTANCES

#include "../flow/flow.hpp"
#include <vulkan/vulkan.hpp>

struct FlowInstances{
    std::vector<VkInstance> instances;
};

namespace flow::vulkan {
    VkInstance createInstance();
}

#endif