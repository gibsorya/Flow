#ifndef FLOW_BUFFERS
#define FLOW_BUFFERS

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

struct FlowFrameBuffers
{
    std::vector<std::vector<vk::Framebuffer>> swapchainFrameBuffers;
};

struct FlowCommandPools
{
    std::vector<vk::CommandPool> commandPools;
};

struct FlowCommandBuffers
{
    std::vector<std::vector<vk::CommandBuffer>> commandBuffers;
};

struct FlowVertexBuffers
{
    std::vector<vk::Buffer> vertexBuffers;
    std::vector<vk::DeviceMemory> bufferMemories;
};

struct FlowIndexBuffers
{
    std::vector<vk::Buffer> indexBuffers;
    std::vector<vk::DeviceMemory> bufferMemories;
};

struct FlowUniformBuffers
{
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
    std::vector<vk::DescriptorPool> descriptorPools;
    std::vector<std::vector<vk::DescriptorSet>> descriptorSets;
    std::vector<std::vector<vk::Buffer>> uniformBuffers;
    std::vector<std::vector<vk::DeviceMemory>> bufferMemories;
};

//TODO restructure this to fit DOD
struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    //Describes at which rate to load data from memory throughout the vertices
    local vk::VertexInputBindingDescription getBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    };

    local std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions.at(0).binding = 0;
        attributeDescriptions.at(0).location = 0;
        attributeDescriptions.at(0).format = vk::Format::eR32G32Sfloat;
        attributeDescriptions.at(0).offset = offsetof(Vertex, pos);
        attributeDescriptions.at(1).binding = 0;
        attributeDescriptions.at(1).location = 1;
        attributeDescriptions.at(1).format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions.at(1).offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

global const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

global const std::vector<u16> indices = {
    0, 1, 2, 2, 3, 0};

namespace flow::vulkan::buffers
{
    Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass);

    Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Device device, vk::CommandPool commandPool, vk::PipelineLayout layout, vk::Extent2D extent, vk::RenderPass renderPass, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, vk::Buffer indexBuffer, std::vector<vk::DescriptorSet> descriptorSets);

    Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue);

    Error createIndexBuffer(vk::Buffer &indexBuffer, vk::DeviceMemory &indexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue);

    Error createBuffer(vk::Buffer &buffer, vk::DeviceMemory &bufferMemory, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Device device, vk::PhysicalDevice physicalDevice);

    Error createUniformBuffers(std::vector<vk::Buffer> &uniformBuffers, std::vector<vk::DeviceMemory> &uniformBufferMemories, vk::Device device, vk::PhysicalDevice physicalDevice, std::vector<vk::Image> swapchainImages);

    Error createDescriptorSetLayout(vk::DescriptorSetLayout &descriptorSetLayout, vk::Device device);

    Error createDescriptorPool(vk::DescriptorPool &descriptorPool, std::vector<vk::Image> swapchainImages, vk::Device device);

    Error createDescriptorSets(std::vector<vk::DescriptorSet> &descriptorSets, vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorPool pool, std::vector<vk::Image> swapchainImages, std::vector<vk::Buffer> uniformBuffers, vk::Device device);

    void updateUniformBuffer(std::vector<vk::DeviceMemory> uniformBufferMemories, u32 currentImage, vk::Extent2D swapExtent, vk::Device device);

    void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue);

    u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice);
}

#endif