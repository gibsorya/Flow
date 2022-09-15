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
  std::vector<vk::CommandPool> graphicsCommandPools;
  std::vector<vk::CommandPool> transferCommandPools;
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

struct FlowVkIndexBuffers
{
  std::vector<vk::Buffer> indexBuffers;
  std::vector<vk::DeviceMemory> indexMemories;
};

namespace flow::vulkan::buffers
{
  Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass);
  Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, u32 queueFamilyIndex);
  Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, vk::Device device, vk::CommandPool commandPool);

  Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue transferQueue, std::array<u32, 2> queueFamilyIndices);
  Error createIndexBuffer(vk::Buffer &indexBuffer, vk::DeviceMemory &indexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue transferQueue, std::array<u32, 2> queueFamilyIndices);

  Error createBuffer(vk::Buffer &buffer, vk::DeviceMemory &bufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size, vk::SharingMode sharingMode, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, std::array<u32, 2> queueFamilyIndices);
  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::Device device, vk::CommandPool commandPool, vk::DeviceSize size, vk::Queue transferQueue);

  Error recordCommandBuffer(vk::CommandBuffer commandBuffer, u32 imageIndex, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, vk::Buffer indexBuffer);

  u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice);
}

#endif