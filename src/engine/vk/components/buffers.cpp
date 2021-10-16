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

    Error createCommandBuffers(std::vector<vk::CommandBuffer> &commandBuffers, std::vector<vk::Framebuffer> swapchainFramebuffers, vk::Device device, vk::CommandPool commandPool, vk::Extent2D extent, vk::RenderPass renderPass, vk::Pipeline graphicsPipeline){
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

            commandBuffers[i].draw(3, 1, 0, 0);

            commandBuffers[i].endRenderPass2(&subpassEndInfo);

            //why doesn't commandBuffers[i].end() return vk::Result? :(
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
                return ERR_CANT_CREATE;
            }
        }


        return SUCCESS;    
    }

}