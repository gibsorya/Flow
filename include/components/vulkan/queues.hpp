#ifndef QUEUES
#define QUEUES

#include <foundation.hpp>
#include <optional>

struct QueueFamilyIndices{
    std::optional<u32> graphicsFamily;
    // std::optional<u32> presentFamily;

    float queuePriority = 1.0f;

    bool isComplete(){
        return graphicsFamily.has_value();
    }
};

namespace flow::vulkan
{
  QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);
}

#endif