#ifndef QUEUES
#define QUEUES

#include "../../flow/flow.hpp"

#include <vulkan/vulkan.h>
#include <optional>

typedef struct QueueFamilyIndicies QueueFamilyIndicies;
struct QueueFamilyIndicies{
    std::optional<u32> graphicsFamily;

    u32 queueFamilyCount = 0;
    VkQueueFamilyProperties2 queueFamilyProperties{};

    bool isComplete(){
        return graphicsFamily.has_value();
    }
};

namespace flow::vulkan::devices{
    QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
}

#endif