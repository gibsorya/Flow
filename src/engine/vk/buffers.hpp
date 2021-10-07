#ifndef FLOW_BUFFERS
#define FLOW_BUFFERS

#include "../flow/flow.hpp"
#include <vulkan/vulkan.hpp>

struct FlowFrameBuffers {
    std::vector<std::vector<VkFramebuffer>> swapchainFrameBuffers;
};

struct FlowCommandPools {
    std::vector<VkCommandPool> commandPools;
};

struct FlowCommandBuffers {
    std::vector<std::vector<VkCommandBuffer>> commandBuffers;
};

namespace flow::vulkan {
    std::vector<VkFramebuffer> createFramebuffers();

    VkCommandPool createCommandPool();

    std::vector<VkCommandBuffer> createCommandBuffers();
}

#endif