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
