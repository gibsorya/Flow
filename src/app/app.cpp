#include "app.hpp"

namespace flow
{

    global FlowContext *flow = nullptr;

    Error setup()
    {

        {

            flow = new FlowContext();

            Error err = vulkan::initialize(flow);

            if (err != SUCCESS)
            {
                flow = nullptr;

                goto error;
            }
        }

        return SUCCESS;

    error:

        if (flow)
        {
            return ERR_CANT_CREATE;
        }

        return ERR_INVALID;
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(flow->flowSurfaces.window))
        {
            glfwPollEvents();
            draw();
        }
    }

    void cleanup()
    {   
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            flow->flowDevices.devices.at(0).destroySemaphore(flow->flowSyncObjects.renderFinishedSemaphores[i], nullptr);
            flow->flowDevices.devices.at(0).destroySemaphore(flow->flowSyncObjects.imageAvailableSemaphores[i], nullptr);
            flow->flowDevices.devices.at(0).destroyFence(flow->flowSyncObjects.inFlightFences[i], nullptr);
        }

        for(auto commandPool : flow->flowCommandPools.commandPools){
            flow->flowDevices.devices.at(0).destroyCommandPool(commandPool);
        }

        for(auto framebuffer : flow->flowFrameBuffers.swapchainFrameBuffers.at(0)){
            flow->flowDevices.devices.at(0).destroyFramebuffer(framebuffer);
        }
        
        for(auto pipeline : flow->flowGraphics.graphicsPipelines){
            flow->flowDevices.devices.at(0).destroyPipeline(pipeline);
        }

        for (auto pipelineLayout : flow->flowGraphics.pipelineLayouts)
		{
			flow->flowDevices.devices.at(0).destroyPipelineLayout(pipelineLayout);
		}

        for(auto renderPass : flow->flowGraphics.renderPasses){
            flow->flowDevices.devices.at(0).destroyRenderPass(renderPass);
        }

        for(vk::ImageView imageView : flow->flowSwaps.swapchainImageViews){
            flow->flowDevices.devices.at(0).destroyImageView(imageView);
        }

        for(size_t i = 0; i < flow->flowSwaps.swapchains.size(); i++){
            flow->flowDevices.devices.at(0).destroySwapchainKHR(flow->flowSwaps.swapchains.at(i));
        }
        
        for(auto device : flow->flowDevices.devices){
            device.destroy();
        }
        
        if (enabledValidationLayers)
        {
            for (auto debugMessenger : flow->flowInstances.debugMessengers)
            {
                vulkan::instances::DestroyDebugUtilsMessengerEXT(flow->flowInstances.instances.at(0), debugMessenger, nullptr);
            }
        }

        int i = 0;
        for (auto instance : flow->flowInstances.instances)
        {
            instance.destroySurfaceKHR(flow->flowSurfaces.surfaces.at(i));
            i++;
        }

        for (auto instance : flow->flowInstances.instances)
        {
            instance.destroy();
        }

        glfwDestroyWindow(flow->flowSurfaces.window);
        glfwTerminate();
    }

    void draw(){
        vk::Result result = flow->flowDevices.devices.at(0).waitForFences(1, &flow->flowSyncObjects.inFlightFences[flow->flowSyncObjects.currentFrame], VK_TRUE, UINT64_MAX);

        u32 imageIndex;

        vk::AcquireNextImageInfoKHR acquireInfo;
        acquireInfo.swapchain = flow->flowSwaps.swapchains.at(0);
        acquireInfo.timeout = UINT64_MAX;
        acquireInfo.semaphore = flow->flowSyncObjects.imageAvailableSemaphores[flow->flowSyncObjects.currentFrame];
        acquireInfo.fence = VK_NULL_HANDLE;
        acquireInfo.deviceMask = 1;

        result = flow->flowDevices.devices.at(0).acquireNextImage2KHR(&acquireInfo, &imageIndex);

        if(result == vk::Result::eErrorOutOfDateKHR){
            // recreateSwapchain();
            return;
        } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR){
            throw std::runtime_error("Failed to acquire swapchain image!");
        }

        if(flow->flowSyncObjects.imagesInFlight[imageIndex]){
            result = flow->flowDevices.devices.at(0).waitForFences(1, &flow->flowSyncObjects.imagesInFlight[flow->flowSyncObjects.currentFrame], VK_TRUE, UINT64_MAX);
        }

        flow->flowSyncObjects.imagesInFlight[imageIndex] = flow->flowSyncObjects.inFlightFences[flow->flowSyncObjects.currentFrame];

        // vk::SubmitInfo2KHR submitInfo;
        vk::SubmitInfo submitInfo;

        std::vector<vk::Semaphore> waitSemaphores = {flow->flowSyncObjects.imageAvailableSemaphores[flow->flowSyncObjects.currentFrame]};
        // std::vector<vk::PipelineStageFlags2KHR> waitStages = { vk::PipelineStageFlagBits2KHR::eColorAttachmentOutput};
        std::vector<vk::PipelineStageFlags> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput};
        // std::vector<vk::SemaphoreSubmitInfoKHR> waitSubmitInfos;

        submitInfo.waitSemaphoreCount = static_cast<u32>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &flow->flowCommandBuffers.commandBuffers.at(0).at(imageIndex);

        //TODO: Change this so it's not dumb. Probably needs two loops, one for waitSemaphores and one for waitStages.
        // ? Does every SemaphoreSubmitInfoKHR need both a semaphore and a stage mask?
        // ? waitSemaphores and waitStages should be same size?
        // for(size_t i = 0; i < waitSemaphores.size(); i++){
        //     vk::SemaphoreSubmitInfoKHR semaphoreSubmitInfo;
        //     semaphoreSubmitInfo.semaphore = waitSemaphores[i];
        //     semaphoreSubmitInfo.stageMask = waitStages[i];
        //     semaphoreSubmitInfo.deviceIndex = 0;
        //     waitSubmitInfos.push_back(semaphoreSubmitInfo);
        // }

        // submitInfo.waitSemaphoreInfoCount = static_cast<u32>(waitSubmitInfos.size());
        // submitInfo.pWaitSemaphoreInfos = waitSubmitInfos.data();

        // vk::CommandBufferSubmitInfoKHR commandBufferSubmitInfo;
        // commandBufferSubmitInfo.commandBuffer = flow->flowCommandBuffers.commandBuffers.at(0).at(imageIndex);
        // commandBufferSubmitInfo.deviceMask = 0;

        // submitInfo.commandBufferInfoCount = 1;
        // submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;

        std::vector<vk::Semaphore> signalSemaphores = {flow->flowSyncObjects.renderFinishedSemaphores[flow->flowSyncObjects.currentFrame]};

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        // std::vector<vk::SemaphoreSubmitInfoKHR> signalSubmitInfos;
        // for(size_t i = 0; i < signalSemaphores.size(); i++){
        //     vk::SemaphoreSubmitInfoKHR semaphoreSubmitInfo;
        //     semaphoreSubmitInfo.semaphore = signalSemaphores[i];
        //     // semaphoreSubmitInfo.stageMask = waitStages[i];
        //     semaphoreSubmitInfo.deviceIndex = 0;
        //     signalSubmitInfos.push_back(semaphoreSubmitInfo);
        // }

        // submitInfo.signalSemaphoreInfoCount = static_cast<u32>(signalSubmitInfos.size());
        // submitInfo.pWaitSemaphoreInfos = signalSubmitInfos.data();
        
        result = flow->flowDevices.devices.at(0).resetFences(1, &flow->flowSyncObjects.inFlightFences[flow->flowSyncObjects.currentFrame]);

        if(flow->flowDevices.graphicsQueues.at(0).submit(1, &submitInfo, flow->flowSyncObjects.inFlightFences[flow->flowSyncObjects.currentFrame]) != vk::Result::eSuccess){
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        // vkQueueSubmit2KHR(flow->flowDevices.graphicsQueues.at(0), 1, &reinterpret_cast<VkSubmitInfo2KHR&>(submitInfo), flow->flowSyncObjects.inFlightFences[flow->flowSyncObjects.currentFrame]);

        vk::PresentInfoKHR presentInfo;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores.data();

        vk::SwapchainKHR swapchains[] = {flow->flowSwaps.swapchains.at(0)};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        result = flow->flowDevices.presentQueues.at(0).presentKHR(&presentInfo);

        if(result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || flow->flowSyncObjects.framebufferResized){
                flow->flowSyncObjects.framebufferResized = false;
                // recreateSwapchain();
        } else if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to present swap chain image!");
        }

        flow->flowSyncObjects.currentFrame = (flow->flowSyncObjects.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

}
