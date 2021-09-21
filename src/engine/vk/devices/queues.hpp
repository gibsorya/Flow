#ifndef QUEUES
#define QUEUES

#include "../../flow/flow.hpp"

#include <vulkan/vulkan.h>
#include <optional>

typedef struct QueueFamilyIndicies QueueFamilyIndicies;
struct QueueFamilyIndicies{
    std::optional<u32> graphicsFamily;

    u32 queueFamilyCount = 0;
    vk::QueueFamilyProperties2 queueFamilyProperties{};

    float queuePriority = 1.0f;

    bool isComplete(){
        return graphicsFamily.has_value();
    }
};

namespace flow::vulkan::devices{
    QueueFamilyIndicies findQueueFamilies(vk::PhysicalDevice device);
}

#endif