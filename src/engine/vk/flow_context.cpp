#include "flow_context.hpp"

namespace flow::vulkan
{
    Error initialize(FlowContext *flow)
    {
        Error err;

        //*Instance + Window
        {
            err = surfaces::initWindow(flow->flowSurfaces.window, flow->flowSurfaces.WIDTH, flow->flowSurfaces.HEIGHT, "Flow Engine");

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window!");

            err = instances::findInstanceExtensions(flow->flowInstances.extensions);

            ERROR_FAIL_COND(err != SUCCESS, ERR_NOT_FOUND, "Failed to find instance extensions!");

            err = instances::createInstance(flow->flowInstances.instances, "Flow", "Flow", flow->flowInstances.extensions);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create instance!");

            err = instances::setupDebugMessenger(flow->flowInstances.instances.at(0), flow->flowInstances.debugMessengers);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to setup debug messenger!");
        }

        return SUCCESS;
    }

    // void mainLoop(FlowContext *flow)
    // {
    //     while (!glfwWindowShouldClose(flow->flowSurfaces.window))
    //     {
    //         glfwPollEvents();
    //     }
    // }

    // void cleanup(FlowContext *flow)
    // {
    //     for(auto instance : flow->flowInstances.instances){
    //         instance.destroy();
    //     }

    //     glfwDestroyWindow(flow->flowSurfaces.window);
    //     glfwTerminate();
    // }
}