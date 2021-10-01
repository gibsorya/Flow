#ifndef FLOW_INSTANCES
#define FLOW_INSTANCES

#include "../flow/flow.hpp"
#include <vulkan/vulkan.hpp>

struct FlowInstances{
    std::vector<VkInstance> instances;
    VkInstance instance;
};

namespace flow::vulkan {
    VkInstance createInstance();
}

// typedef struct FlowInstances FlowInstances;

// namespace flow {
// struct FlowInstances {
// public:
//     // [[nodiscard]] const VkInstance& findInstance() const;
//     // [[nodiscard]] const std::vector<VkInstance>& getInstances() const;
//     [[nodiscard]] const VkInstance& getInstance() const;
//     void createInstance();
//     // VkInstance createInstances();
//     // FlowInstances();
//     // std::shared_ptr<VkInstance> flowInstance = std::make_shared<VkInstance>();
    
//     // vk::Instance vkInstance;

// private:
//     VkInstance instance;
//     std::vector<VkInstance> instances;
// };

// }

// namespace flow::vulkan{
//     // void startFlow();
//     void createInstance();
    
// }

#endif