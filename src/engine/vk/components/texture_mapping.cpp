#include "texture_mapping.hpp"
#include "buffers.hpp"
#include <stb_image.h>


namespace flow::vulkan::textures
{
    Error createTextureImage(vk::Image &textureImage, vk::DeviceMemory &textureImageMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load("data/textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        vk::DeviceSize imageSize = texWidth * texHeight * 4;

        ERROR_FAIL_COND(!pixels, ERR_NOT_FOUND, "Failed to load texture image!");

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        buffers::createBuffer(stagingBuffer, stagingBufferMemory, imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device, physicalDevice);

        void* data;
        vk::Result result = device.mapMemory(stagingBufferMemory, 0, imageSize, {}, &data);
            memcpy(data, pixels, static_cast<size_t>(imageSize));
        device.unmapMemory(stagingBufferMemory);

        stbi_image_free(pixels);

        Error err = createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory, device, physicalDevice);
        if(err != SUCCESS)
        {
            return ERR_CANT_CREATE;
        }

        transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, device, commandPool, graphicsQueue);
        copyBufferToImage(stagingBuffer, textureImage, static_cast<u32>(texWidth), static_cast<u32>(texHeight), device, commandPool, graphicsQueue);
        transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, device, commandPool, graphicsQueue);

        device.destroyBuffer(stagingBuffer);
        device.freeMemory(stagingBufferMemory);

        return SUCCESS;
    }

    Error createImage(u32 width, u32 height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &imageMemory, vk::Device device, vk::PhysicalDevice physicalDevice) {
        vk::ImageCreateInfo imageInfo{};
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = static_cast<u32>(width);
        imageInfo.extent.height = static_cast<u32>(height);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = usage;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.flags = {};

        if(device.createImage(&imageInfo, nullptr, &image) != vk::Result::eSuccess)
        {
            return ERR_CANT_CREATE;
        }

        vk::MemoryRequirements memRequirements;
        device.getImageMemoryRequirements(image, &memRequirements);

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = buffers::findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

        ERROR_FAIL_COND(device.allocateMemory(&allocInfo, nullptr, &imageMemory) != vk::Result::eSuccess, ERR_INVALID, "Failed to allocate image memory!");

        device.bindImageMemory(image, imageMemory, 0);
        
        return SUCCESS;
    }

    Error createTextureImageView(vk::ImageView &textureImageView, vk::Device device, vk::Image textureImage) {
        if(createImageView(textureImageView, textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, device) != SUCCESS)
        {
            return ERR_CANT_CREATE;
        }
        
        return SUCCESS;
    }

    Error createTextureSampler(vk::Sampler &textureSampler, vk::Device device, vk::PhysicalDevice physicalDevice) {
        vk::SamplerCreateInfo samplerInfo{};

        samplerInfo.magFilter = vk::Filter::eLinear;
        samplerInfo.minFilter = vk::Filter::eLinear;
        samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
        samplerInfo.anisotropyEnable = VK_TRUE;

        vk::PhysicalDeviceProperties properties;
        physicalDevice.getProperties(&properties);

        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = vk::CompareOp::eAlways;
        samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if(device.createSampler(&samplerInfo, nullptr, &textureSampler) != vk::Result::eSuccess)
        {
            return ERR_CANT_CREATE;
        }
        
        return SUCCESS;
    }

    Error createImageView(vk::ImageView &imageView, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, vk::Device device) {
        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.image = image;
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if(device.createImageView(&viewInfo, nullptr, &imageView) != vk::Result::eSuccess)
        {
            ERROR_FAIL(ERR_CANT_CREATE, "Failed to create image view!");
        }
        
        return SUCCESS;
    }

    void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        vk::CommandBuffer commandBuffer = buffers::beginSingleTimeCommands(device, commandPool);

        vk::ImageMemoryBarrier barrier{};
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = {};
        barrier.dstAccessMask = {};

        if(newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
        {
            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

            if(buffers::hasStencilComponent(format))
            {
                barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
            }
        } else {
            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        }

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        if(oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
        {
            barrier.srcAccessMask = {};
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
        } else if(oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
        } else if(oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
            barrier.srcAccessMask = {};
            barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
        } else {
            throw std::invalid_argument("Unsupported layout transition!");
        }

        commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, 0, nullptr, 0, nullptr, 1, &barrier);

        buffers::endSingleTimeCommands(commandBuffer, device, commandPool, graphicsQueue);
    }

    void copyBufferToImage(vk::Buffer buffer, vk::Image image, u32 width, u32 height, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        vk::CommandBuffer commandBuffer = buffers::beginSingleTimeCommands(device, commandPool);

        vk::BufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {{0, 0, 0}};
        region.imageExtent = {{ width, height, 1}};

        commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);
        
        buffers::endSingleTimeCommands(commandBuffer, device, commandPool, graphicsQueue);
    }

}