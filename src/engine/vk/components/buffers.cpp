#include "buffers.hpp"
#include <engine/vk/components/queues.hpp>
#include "texture_mapping.hpp"

namespace flow::vulkan::buffers
{

    Error createFramebuffers(std::vector<vk::Framebuffer> &framebuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::ImageView depthImageView, vk::Extent2D swapExtent, vk::RenderPass renderpass)
    {
        framebuffers.resize(swapchainImageViews.size());

        for (size_t i = 0; i < swapchainImageViews.size(); i++)
        {
            std::array<vk::ImageView, 2> attachments = {
                swapchainImageViews.at(i),
                depthImageView};

            vk::FramebufferCreateInfo createInfo;
            createInfo.renderPass = renderpass;
            createInfo.attachmentCount = static_cast<u32>(attachments.size());
            createInfo.pAttachments = attachments.data();
            createInfo.width = swapExtent.width;
            createInfo.height = swapExtent.height;
            createInfo.layers = 1;

            if (device.createFramebuffer(&createInfo, nullptr, &framebuffers[i]) != vk::Result::eSuccess)
            {
                return ERR_CANT_CREATE;
            }
        }

        return SUCCESS;
    }

    Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
    {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

        vk::CommandPoolCreateInfo poolInfo;
        poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
        poolInfo.flags = {};

        if (device.createCommandPool(&poolInfo, nullptr, &commandPool) != vk::Result::eSuccess)
        {
            return ERR_CANT_CREATE;
        }

        return SUCCESS;
    };

    Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Device device, vk::CommandPool commandPool, vk::PipelineLayout layout, vk::Extent2D extent, vk::RenderPass renderPass, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, vk::Buffer indexBuffer, std::vector<vk::DescriptorSet> descriptorSets)
    {
        commandBuffers.resize(swapchainFramebuffers.size());

        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = (u32)commandBuffers.size();
        allocInfo.commandPool = commandPool;

        ERROR_FAIL_COND(device.allocateCommandBuffers(&allocInfo, commandBuffers.data()) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to allocate command buffer!");

        for (size_t i = 0; i < commandBuffers.size(); i++)
        {
            vk::CommandBufferBeginInfo beginInfo;
            beginInfo.flags = {};
            beginInfo.pInheritanceInfo = nullptr;

            ERROR_FAIL_COND(commandBuffers[i].begin(&beginInfo) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to begin recording command buffer!");

            std::array<vk::ClearValue, 2> clearValues{};
            clearValues[0].color = {std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};
            clearValues[1].depthStencil = { { 1.0f, 0 } };

            vk::RenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapchainFramebuffers.at(i);
            renderPassInfo.renderArea.offset.x = 0;
            renderPassInfo.renderArea.offset.y = 0;
            renderPassInfo.renderArea.extent = extent;
            renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vk::SubpassBeginInfo subpassInfo;
            subpassInfo.contents = vk::SubpassContents::eInline;

            vk::SubpassEndInfo subpassEndInfo;

            commandBuffers[i].beginRenderPass2(&renderPassInfo, &subpassInfo);

            commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

            vk::Buffer vertexBuffers[] = {vertexBuffer};
            vk::DeviceSize offsets[] = {0};

            commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);

            commandBuffers[i].bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16);

            commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, 1, &descriptorSets.at(i), 0, nullptr);
            commandBuffers[i].drawIndexed(static_cast<u32>(indices.size()), 1, 0, 0, 0);

            commandBuffers[i].endRenderPass2(&subpassEndInfo);

            //why doesn't commandBuffers[i].end() return vk::Result? :(
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                return ERR_CANT_CREATE;
            }
        }

        return SUCCESS;
    }

    Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue)
    {
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        Error error = createBuffer(stagingBuffer, stagingBufferMemory, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device, physicalDevice);
        
        if(error != SUCCESS){
            return ERR_CANT_CREATE;
        }

        void *data;
        vk::Result result = device.mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
            memcpy(data, vertices.data(), (size_t)bufferSize);
        device.unmapMemory(stagingBufferMemory);

        createBuffer(vertexBuffer, vertexBufferMemory, bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, device, physicalDevice);

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize, device, commandPool, graphicsQueue);

        device.destroyBuffer(stagingBuffer, nullptr);
        device.freeMemory(stagingBufferMemory, nullptr);

        return SUCCESS;
    }

    Error createIndexBuffer(vk::Buffer &indexBuffer, vk::DeviceMemory &indexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        Error error = createBuffer(stagingBuffer, stagingBufferMemory, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device, physicalDevice);
        
        if(error != SUCCESS){
            return ERR_CANT_CREATE;
        }

        void *data;
        vk::Result result = device.mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
            memcpy(data, indices.data(), (size_t)bufferSize);
        device.unmapMemory(stagingBufferMemory);

        createBuffer(indexBuffer, indexBufferMemory, bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, device, physicalDevice);

        copyBuffer(stagingBuffer, indexBuffer, bufferSize, device, commandPool, graphicsQueue);

        device.destroyBuffer(stagingBuffer, nullptr);
        device.freeMemory(stagingBufferMemory, nullptr);
        
        return SUCCESS;
    }

    Error createBuffer(vk::Buffer &buffer, vk::DeviceMemory &bufferMemory, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Device device, vk::PhysicalDevice physicalDevice)
    {
        vk::BufferCreateInfo bufferInfo{};
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        if (device.createBuffer(&bufferInfo, nullptr, &buffer) != vk::Result::eSuccess)
        {
            return ERR_CANT_CREATE;
        }

        vk::MemoryRequirements memRequirements;
        device.getBufferMemoryRequirements(buffer, &memRequirements);

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

        if (device.allocateMemory(&allocInfo, nullptr, &bufferMemory) != vk::Result::eSuccess)
        {
            ERROR_FAIL(ERR_INVALID, "Failed to allocate vertex buffer memory!");
        }

        device.bindBufferMemory(buffer, bufferMemory, 0);

        return SUCCESS;
    }

    Error createUniformBuffers(std::vector<vk::Buffer> &uniformBuffers, std::vector<vk::DeviceMemory> &uniformBufferMemories, vk::Device device, vk::PhysicalDevice physicalDevice, std::vector<vk::Image> swapchainImages) {
        vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(swapchainImages.size());
        uniformBufferMemories.resize(swapchainImages.size());

        Error error;

        for(size_t i = 0; i < swapchainImages.size(); i++)
        {
            error = createBuffer(uniformBuffers[i], uniformBufferMemories[i], bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device, physicalDevice);
            if(error != SUCCESS) {
                return ERR_CANT_CREATE;
            }
        }
        
        return SUCCESS;
    }

    Error createDescriptorSetLayout(vk::DescriptorSetLayout &descriptorSetLayout, vk::Device device) {
        vk::DescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

        std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.bindingCount = static_cast<u32>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if(device.createDescriptorSetLayout(&layoutInfo, nullptr, &descriptorSetLayout) != vk::Result::eSuccess){
            return ERR_CANT_CREATE;
        }
        
        return SUCCESS;
    }

    Error createDescriptorPool(vk::DescriptorPool &descriptorPool, std::vector<vk::Image> swapchainImages, vk::Device device) {

        std::array<vk::DescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
        poolSizes[0].descriptorCount = static_cast<u32>(swapchainImages.size());
        poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
        poolSizes[1].descriptorCount = static_cast<u32>(swapchainImages.size());

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<u32>(swapchainImages.size());

        if(device.createDescriptorPool(&poolInfo, nullptr, &descriptorPool) != vk::Result::eSuccess)
        {
            return ERR_CANT_CREATE;
        }
        
        return SUCCESS;
    }

    void updateUniformBuffer(std::vector<vk::DeviceMemory> uniformBufferMemories, u32 currentImage, vk::Extent2D swapExtent, vk::Device device) {
        local auto startTime = std::chrono::high_resolution_clock::now();
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), swapExtent.width / (float) swapExtent.height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        void* data;
        vk::Result result = device.mapMemory(uniformBufferMemories.at(currentImage), 0, sizeof(ubo), {}, &data);
            memcpy(data, &ubo, sizeof(ubo));
        device.unmapMemory(uniformBufferMemories.at(currentImage));
    }

    void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        vk::CommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

        vk::BufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;

        commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer, device, commandPool, graphicsQueue);
    }

    vk::CommandBuffer beginSingleTimeCommands(vk::Device device, vk::CommandPool commandPool) {
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        vk::CommandBuffer commandBuffer;

        vk::Result result = device.allocateCommandBuffers(&allocInfo, &commandBuffer);

        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        result = commandBuffer.begin(&beginInfo);

        return commandBuffer;
    }

    void endSingleTimeCommands(vk::CommandBuffer commandBuffer, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        commandBuffer.end();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vk::Result result = graphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE);
        graphicsQueue.waitIdle();

        device.freeCommandBuffers(commandPool, 1, &commandBuffer);       
    }

    u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice)
    {
        vk::PhysicalDeviceMemoryProperties2 memProperties;
        physicalDevice.getMemoryProperties2(&memProperties);

        for (u32 i = 0; i < memProperties.memoryProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    Error createDescriptorSets(std::vector<vk::DescriptorSet> &descriptorSets, vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorPool pool, std::vector<vk::Image> swapchainImages, std::vector<vk::Buffer> uniformBuffers, vk::Device device, vk::ImageView textureImageView, vk::Sampler textureSampler) {
        std::vector<vk::DescriptorSetLayout> layouts(swapchainImages.size(), descriptorSetLayout);
        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = static_cast<u32>(swapchainImages.size());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(swapchainImages.size());
        ERROR_FAIL_COND(device.allocateDescriptorSets(&allocInfo, descriptorSets.data()) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to allocate descriptor sets!");
        
        for(size_t i = 0; i < swapchainImages.size(); i++)
        {
            vk::DescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers.at(i);
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            vk::DescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            imageInfo.imageView = textureImageView;
            imageInfo.sampler = textureSampler;

            std::array<vk::WriteDescriptorSet, 2> descriptorWrites{};
            descriptorWrites[0].dstSet = descriptorSets.at(i);
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].dstSet = descriptorSets.at(i);
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            device.updateDescriptorSets(static_cast<u32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
        
        return SUCCESS;
    }

    Error createDepthResources(vk::Image &depthImage, vk::DeviceMemory &depthImageMemory, vk::ImageView &depthImageView, vk::Device device, vk::PhysicalDevice physicalDevice, vk::Extent2D swapExtent, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        vk::Format depthFormat = findDepthFormat(physicalDevice);

        Error error = textures::createImage(swapExtent.width, swapExtent.height, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory, device, physicalDevice);

        if(error != SUCCESS)
        {
            ERROR_FAIL(ERR_CANT_CREATE, "Cannot create image for depth resources");
        }

        error = textures::createImageView(depthImageView, depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, device);

        if(error != SUCCESS)
        {
            ERROR_FAIL(ERR_CANT_CREATE, "Cannot create image view for depth resources");
        }

        textures::transitionImageLayout(depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, device, commandPool, graphicsQueue);
        
        return SUCCESS;
    }

    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, vk::PhysicalDevice physicalDevice) {
        for(vk::Format format : candidates) {
            vk::FormatProperties props;
            physicalDevice.getFormatProperties(format, &props);

            if(tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if(tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }
        
        throw std::runtime_error("Failed to find supported format!");
    }

    vk::Format findDepthFormat(vk::PhysicalDevice physicalDevice) {    
        return findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment, physicalDevice);    
    }

    bool hasStencilComponent(vk::Format format)
    {
        return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
    }

}