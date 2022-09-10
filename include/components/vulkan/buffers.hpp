#ifndef FLOW_VK_BUFFERS
#define FLOW_VK_BUFFERS

#include <foundation.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

struct FlowVkFrameBuffers
{
    std::vector<std::vector<vk::Framebuffer>> swapchainFrameBuffers;
};

namespace flow::vulkan::buffers
{
  Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass);
}

#endif