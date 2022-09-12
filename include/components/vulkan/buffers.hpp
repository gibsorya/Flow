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

struct FlowVkCommandPools
{
  std::vector<vk::CommandPool> commandPools;
};

struct FlowVkCommandBuffers
{
  std::vector<std::vector<vk::CommandBuffer>> commandBuffers;
};

namespace flow::vulkan::buffers
{
  Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass);
  Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
  Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, vk::Device device, vk::CommandPool commandPool);
  Error createCommandBuffer(vk::CommandBuffer &commandBuffer, vk::Device device, vk::CommandPool commandPool);

  Error recordCommandBuffer(vk::CommandBuffer commandBuffer, u32 imageIndex, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Pipeline graphicsPipeline);
}

#endif