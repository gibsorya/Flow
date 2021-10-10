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

            // vulkan::mainLoop(flow);
            // vulkan::cleanup(flow);
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
        if (enabledValidationLayers)
        {
            for (auto debugMessenger : flow->flowInstances.debugMessengers)
            {
                vulkan::instances::DestroyDebugUtilsMessengerEXT(flow->flowInstances.instances.at(0), debugMessenger, nullptr);
            }
        }

        for (auto instance : flow->flowInstances.instances)
        {
            instance.destroy();
        }

        glfwDestroyWindow(flow->flowSurfaces.window);
        glfwTerminate();
    }

}
