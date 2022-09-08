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

    return SUCCESS;
  }
}