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

struct FlowVkVertexBuffers
{
  std::vector<vk::Buffer> vertexBuffers;
  std::vector<vk::DeviceMemory> vertexMemories;
};

namespace flow::vulkan::buffers
{
  Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass);
  Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
  Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, vk::Device device, vk::CommandPool commandPool);

  Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue);
  Error createBuffer(vk::Buffer &buffer, vk::DeviceMemory &bufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size, vk::SharingMode sharingMode, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::Device device, vk::CommandPool commandPool, vk::DeviceSize size, vk::Queue graphicsQueue);
  Error recordCommandBuffer(vk::CommandBuffer commandBuffer, u32 imageIndex, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer);

  u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice);
}

#endif