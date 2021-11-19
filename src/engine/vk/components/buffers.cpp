#include "buffers.hpp"
#include <engine/vk/components/queues.hpp>

namespace flow::vulkan::buffers {

    Error createFramebuffers(std::vector<vk::Framebuffer> &framebuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass){
        framebuffers.resize(swapchainImageViews.size());

        for(size_t i = 0; i < swapchainImageViews.size(); i++){
            vk::ImageView attachments[] = {
                swapchainImageViews.at(i)
            };

            vk::FramebufferCreateInfo createInfo;
            createInfo.renderPass = renderpass;
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = attachments;
            createInfo.width = swapExtent.width;
            createInfo.height = swapExtent.height;
            createInfo.layers = 1;

            if(device.createFramebuffer(&createInfo, nullptr, &framebuffers[i]) != vk::Result::eSuccess){
                return ERR_CANT_CREATE;
            }
        }

        return SUCCESS;
    }

    Error createCommandPool(vk::CommandPool &commandPool, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface){
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

        vk::CommandPoolCreateInfo poolInfo;
        poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
        poolInfo.flags = {};

        if(device.createCommandPool(&poolInfo, nullptr, &commandPool) != vk::Result::eSuccess){
            return ERR_CANT_CREATE;
        }

        return SUCCESS;
    };

    Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Device device, vk::CommandPool commandPool, vk::Extent2D extent, vk::RenderPass renderPass, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer){
        commandBuffers.resize(swapchainFramebuffers.size());

        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = (u32)commandBuffers.size();
        allocInfo.commandPool = commandPool;

        ERROR_FAIL_COND(device.allocateCommandBuffers(&allocInfo, commandBuffers.data()) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to allocate command buffer!");

        for(size_t i = 0; i < commandBuffers.size(); i++){
            vk::CommandBufferBeginInfo beginInfo;
            beginInfo.flags = {};
            beginInfo.pInheritanceInfo = nullptr;

            ERROR_FAIL_COND(commandBuffers[i].begin(&beginInfo) != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to begin recording command buffer!");

            vk::ClearValue clearColor;
            clearColor.color = { std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f} };

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

            commandBuffers[i].draw(3, 1, 0, 0);

            commandBuffers[i].endRenderPass2(&subpassEndInfo);

            //why doesn't commandBuffers[i].end() return vk::Result? :(
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
                return ERR_CANT_CREATE;
            }
        }


        return SUCCESS;    
    }

    Error createVertexBuffer(vk::Buffer &vertexBuffer, vk::DeviceMemory &vertexBufferMemory, vk::Device device, vk::PhysicalDevice physicalDevice) {
        // vk::BufferCreateInfo bufferInfo{};
        // bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        // bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        // bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        // if(device.createBuffer(&bufferInfo, nullptr, &vertexBuffer) != vk::Result::eSuccess) {
        //     return ERR_CANT_CREATE;
        // }

        // vk::MemoryRequirements memRequirements;
        // device.getBufferMemoryRequirements(vertexBuffer, &memRequirements);

        // vk::MemoryAllocateInfo allocInfo{};
        // allocInfo.allocationSize = memRequirements.size;
        // allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, physicalDevice);

        // if(device.allocateMemory(&allocInfo, nullptr, &vertexBufferMemory) != vk::Result::eSuccess) {
        //     ERROR_FAIL(ERR_INVALID, "Failed to allocate vertex buffer memory!");
        // }

        // device.bindBufferMemory(vertexBuffer, vertexBufferMemory, 0);
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        createBuffer(vertexBuffer, vertexBufferMemory, bufferSize, vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device, physicalDevice);

        void* data;
        device.mapMemory(vertexBufferMemory, 0, bufferSize, {}, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        device.unmapMemory(vertexBufferMemory);

        return SUCCESS;
    }

    Error createBuffer(vk::Buffer &buffer, vk::DeviceMemory &bufferMemory, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Device device, vk::PhysicalDevice physicalDevice) {
        vk::BufferCreateInfo bufferInfo{};
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        if(device.createBuffer(&bufferInfo, nullptr, &buffer) != vk::Result::eSuccess) {
            return ERR_CANT_CREATE;
        }

        vk::MemoryRequirements memRequirements;
        device.getBufferMemoryRequirements(buffer, &memRequirements);

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

        if(device.allocateMemory(&allocInfo, nullptr, &bufferMemory) != vk::Result::eSuccess) {
            ERROR_FAIL(ERR_INVALID, "Failed to allocate vertex buffer memory!");
        }

        device.bindBufferMemory(buffer, bufferMemory, 0);
        
        return SUCCESS;
    }

    u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice) {
        vk::PhysicalDeviceMemoryProperties2 memProperties;
        physicalDevice.getMemoryProperties2(&memProperties);

        for(u32 i = 0; i < memProperties.memoryProperties.memoryTypeCount; i++){
            if((typeFilter & (1 << i)) && (memProperties.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties){
                return i;
            }
        }
        
        throw std::runtime_error("Failed to find suitable memory type!");
    }

}