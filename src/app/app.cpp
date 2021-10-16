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
        }
    }

    void cleanup()
    {
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

}
