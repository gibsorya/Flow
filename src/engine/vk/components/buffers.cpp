#include "buffers.hpp"
#include <engine/vk/components/queues.hpp>

namespace flow::vulkan::buffers
{

    Error createFramebuffers(std::vector<vk::Framebuffer> &framebuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass)
    {
        framebuffers.resize(swapchainImageViews.size());

        for (size_t i = 0; i < swapchainImageViews.size(); i++)
        {
            vk::ImageView attachments[] = {
                swapchainImageViews.at(i)};

            vk::FramebufferCreateInfo createInfo;
            createInfo.renderPass = renderpass;
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = attachments;
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

            vk::ClearValue clearColor;
            clearColor.color = {std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};

            vk::RenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapchainFramebuffers.at(i);
            renderPassInfo.renderArea.offset.x = 0;
            renderPassInfo.renderArea.offset.y = 0;
            renderPassInfo.renderArea.extent = extent;
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

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

        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if(device.createDescriptorSetLayout(&layoutInfo, nullptr, &descriptorSetLayout) != vk::Result::eSuccess){
            return ERR_CANT_CREATE;
        }
        
        return SUCCESS;
    }

    Error createDescriptorPool(vk::DescriptorPool &descriptorPool, std::vector<vk::Image> swapchainImages, vk::Device device) {
        vk::DescriptorPoolSize poolSize{};
        poolSize.type = vk::DescriptorType::eUniformBuffer;
        poolSize.descriptorCount = static_cast<u32>(swapchainImages.size());

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
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
        device.mapMemory(uniformBufferMemories.at(currentImage), 0, sizeof(ubo), {}, &data);
            memcpy(data, &ubo, sizeof(ubo));
        device.unmapMemory(uniformBufferMemories.at(currentImage));
    }

    void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size, vk::Device device, vk::CommandPool commandPool, vk::Queue graphicsQueue) {
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        vk::CommandBuffer commandBuffer;
        vk::Result result =device.allocateCommandBuffers(&allocInfo, &commandBuffer);
        
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        result = commandBuffer.begin(&beginInfo);

        vk::BufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;

        commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

        commandBuffer.end();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        result = graphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE);
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

    Error createDescriptorSets(std::vector<vk::DescriptorSet> &descriptorSets, vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorPool pool, std::vector<vk::Image> swapchainImages, std::vector<vk::Buffer> uniformBuffers, vk::Device device) {
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

            vk::WriteDescriptorSet descriptorWrite{};
            descriptorWrite.dstSet = descriptorSets.at(i);
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr;
            descriptorWrite.pTexelBufferView = nullptr;

            device.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
        }
        
        return SUCCESS;
    }

}