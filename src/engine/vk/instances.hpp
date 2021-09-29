#ifndef FLOW_INSTANCES
#define FLOW_INSTANCES

#include "../flow/flow.hpp"
#include <vulkan/vulkan.h>

typedef struct FlowInstances FlowInstances;
struct FlowInstances {
    //std::unique_ptr<std::vector<VkInstance>> instances = std::make_unique<std::vector<VkInstance>>();
    // FlowInstances();
    // std::unique_ptr<VkInstance> instance;
    VkInstance instance;
    // [[nodiscard]] const std::vector<VkInstance>& getInstances() const;
private:
    std::vector<VkInstance> instances;
};

namespace flow::vulkan{
    // void startFlow();
    VkInstance createInstance();
}

#endif