// #include "buffers.hpp"
// #include "../root.hpp"

// namespace flow::vulkan {

//     std::vector<VkFramebuffer> createFramebuffers() {
//         std::vector<VkFramebuffer> framebuffers;

//         framebuffers.resize(root.imageViews.swapchainImageViews.size());

//         for(size_t i = 0; i < root.imageViews.swapchainImageViews.size(); i++){
//             VkImageView attachments[] = {
//                 root.imageViews.swapchainImageViews.at(i)
//             };

//             VkFramebufferCreateInfo framebufferInfo{};
//             framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//             framebufferInfo.renderPass = root.flowRenderPasses.renderPasses.at(0);
//             framebufferInfo.attachmentCount = 1;
//             framebufferInfo.pAttachments = attachments;
//             framebufferInfo.width = root.flowSwaps.swapchainExtents.at(0).width;
//             framebufferInfo.height = root.flowSwaps.swapchainExtents.at(0).height;
//             framebufferInfo.layers = 1;

//             if(vkCreateFramebuffer(root.flowDevices.devices.at(0), &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS){
//                 throw std::runtime_error("Failed to create framebuffer!");
//             }
//         }

//         return framebuffers;
    
//     }

//     VkCommandPool createCommandPool() {
//         VkCommandPool commandPool;

//         QueueFamilyIndicies indices = findQueueFamilies(root.flowDevices.physicalDevice);

//         VkCommandPoolCreateInfo poolInfo{};
//         poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//         poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
//         poolInfo.flags = 0;

//         if(vkCreateCommandPool(root.flowDevices.devices.at(0), &poolInfo, nullptr, &commandPool) != VK_SUCCESS){
//             throw std::runtime_error("Failed to create command pool!");
//         }
        
//         return commandPool;
//     }

//     std::vector<VkCommandBuffer> createCommandBuffers() {
//         std::vector<VkCommandBuffer> commandBuffers;

//         commandBuffers.resize(root.flowFrameBuffers.swapchainFrameBuffers.at(0).size());

//         VkCommandBufferAllocateInfo allocInfo{};
//         allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//         allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//         allocInfo.commandBufferCount = (u32)commandBuffers.size();
//         allocInfo.commandPool = root.flowCommandPools.commandPools.at(0);

//         if(vkAllocateCommandBuffers(root.flowDevices.devices.at(0), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
//             throw std::runtime_error("Failed to allocate command buffers!");
//         }

//         for(size_t i = 0; i < commandBuffers.size(); i++) {
//             VkCommandBufferBeginInfo beginInfo{};
//             beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//             beginInfo.flags = 0;
//             beginInfo.pInheritanceInfo = nullptr;

//             if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
//                 throw std::runtime_error("Failed to begin recording command buffer!");
//             }

//             VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

//             VkRenderPassBeginInfo renderPassInfo{};
//             renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//             renderPassInfo.renderPass = root.flowRenderPasses.renderPasses.at(0);
//             renderPassInfo.framebuffer = root.flowFrameBuffers.swapchainFrameBuffers.at(0).at(i);
//             renderPassInfo.renderArea.offset = {0, 0};
//             renderPassInfo.renderArea.extent = root.flowSwaps.swapchainExtents.at(0);
//             renderPassInfo.clearValueCount = 1;
//             renderPassInfo.pClearValues = &clearColor;

//             VkSubpassBeginInfo subpassBeginInfo{};
//             subpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;
//             subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;

//             VkSubpassEndInfo subpassEndInfo{};
//             subpassEndInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO;

//             vkCmdBeginRenderPass2(commandBuffers[i], &renderPassInfo, &subpassBeginInfo);

//             vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, root.graphicsPipelines.graphicsPipelines.at(0));

//             vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

//             vkCmdEndRenderPass2(commandBuffers[i], &subpassEndInfo);

//             if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
//                 throw std::runtime_error("Failed to record command buffer!");
//             }
//         }
        
//         return commandBuffers;
//     }
    
// }
