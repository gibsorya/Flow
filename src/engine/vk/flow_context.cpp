#include "flow_context.hpp"

namespace flow::vulkan
{
    /*

    * Initialize function for main Flow runtime. For examples, create your own init function.

    */
    Error initialize(FlowContext *flow)
    {
        Error err;

        //*Instance + Window
        {
            err = surfaces::initWindow(flow->flowSurfaces.window, flow->flowSurfaces.WIDTH, flow->flowSurfaces.HEIGHT, "Flow Engine");

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window!");

            flow->flowInstances.extensions = instances::findInstanceExtensions();

            err = instances::createInstance(flow->flowInstances.instances, "Flow", "Flow", flow->flowInstances.extensions);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create instance!");

            err = surfaces::createSurface(flow->flowSurfaces.surfaces, flow->flowInstances.instances.at(0), flow->flowSurfaces.window);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window surface!");

            err = instances::setupDebugMessenger(flow->flowInstances.instances.at(0), flow->flowInstances.debugMessengers);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to setup debug messenger!");
        }

        //*Devices
        {
            err = devices::pickPhysicalDevice(flow->flowDevices.physicalDevices, flow->flowInstances.instances.at(0), flow->flowSurfaces.surfaces.at(0));

            ERROR_FAIL_COND(err != SUCCESS, ERR_NOT_FOUND, "Cannot pick suitable GPU!");

            vk::Queue graphicsQueue;
            vk::Queue presentQueue;

            err = devices::createLogicalDevice(flow->flowDevices.devices, flow->flowDevices.physicalDevices.at(0), flow->flowSurfaces.surfaces.at(0), graphicsQueue, presentQueue);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create logical device!");

            flow->flowDevices.graphicsQueues.push_back(graphicsQueue);
            flow->flowDevices.presentQueues.push_back(presentQueue);
        }

        //*Swapchains
        {
            err = swapchains::createSwapchain(flow->flowSwaps, flow->flowDevices.devices.at(0), flow->flowDevices.physicalDevices.at(0), flow->flowSurfaces.surfaces.at(0), flow->flowSurfaces.window);

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create swapchain!");

            err = swapchains::createImageViews(flow->flowSwaps, flow->flowDevices.devices.at(0));

            ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create image views!");
        }

        return SUCCESS;
    }
}