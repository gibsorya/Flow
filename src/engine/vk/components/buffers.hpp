#ifndef FLOW_BUFFERS
#define FLOW_BUFFERS

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>

struct FlowFrameBuffers {
    std::vector<std::vector<vk::Framebuffer>> swapchainFrameBuffers;
};

struct FlowCommandPools {
    std::vector<vk::CommandPool> commandPools;
};

struct FlowCommandBuffers {
    std::vector<std::vector<vk::CommandBuffer>> commandBuffers;
};

namespace flow::vulkan::buffers {
    Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass);

    Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffer);
}

#endif