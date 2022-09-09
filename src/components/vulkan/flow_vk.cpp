#include <flow_vk.hpp>

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext){
    Error err;

    err = surfaces::initWindow(vkContext->surfaces.window, vkContext->surfaces.WIDTH, vkContext->surfaces.HEIGHT, "Flow Engine");

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window!");

    vkContext->instances.extensions = instances::findInstanceExtensions();

    err = instances::createInstance(vkContext->instances.instances, "Flow", "Flow", vkContext->instances.extensions);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create instance!");

    err = instances::setupDebugMessenger(vkContext->instances.instances.at(0), vkContext->instances.debugMessengers);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to setup debug messenger!");

    err = devices::pickPhysicalDevice(vkContext->devices.physicalDevices, vkContext->instances.instances.at(0));

    ERROR_FAIL_COND(err != SUCCESS, ERR_NOT_FOUND, "Failed to find physical device!");

    vk::Queue graphicsQueue;

    err = devices::createLogicalDevice(vkContext->devices.devices, vkContext->devices.physicalDevices.at(0), graphicsQueue);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create logical device!");

    vkContext->devices.graphicsQueues.push_back(graphicsQueue);

    return SUCCESS;
  }
}