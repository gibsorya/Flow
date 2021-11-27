#ifndef TEXTURE_MAPPING
#define TEXTURE_MAPPING

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>


#define STB_IMAGE_IMPLEMENTATION

struct FlowTextures {
    std::vector<vk::Image> textureImages;
    std::vector<vk::DeviceMemory> textureImageMemories;

    std::vector<vk::ImageView> textureImageViews;
    std::vector<vk::Sampler> textureSamplers;
};

namespace flow::vulkan::textures
{
    Error createTextureImage(vk::Image &textureImage, vk::DeviceMemory &textureImageMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue);

    Error createImage(u32 width, u32 height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &imageMemory, vk::Device device, vk::PhysicalDevice physicalDevice);

    Error createTextureImageView(vk::ImageView &textureImageView, vk::Device device, vk::Image textureImage);

    Error createTextureSampler(vk::Sampler &textureSampler, vk::Device device, vk::PhysicalDevice physicalDevice);

    Error createImageView(vk::ImageView &imageView, vk::Image image, vk::Format format, vk::Device device);

    void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue);

    void copyBufferToImage(vk::Buffer buffer, vk::Image image, u32 width, u32 height, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue);
}

#endif