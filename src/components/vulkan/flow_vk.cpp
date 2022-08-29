#include <flow_vk.hpp>

namespace flow::vulkan {
  Error initalize(FlowVkContext *vkContext){
    Error err;

    std::cout << "INIT Vulkan" << std::endl;
    err = surfaces::initWindow(vkContext->surfaces.window, vkContext->surfaces.WIDTH, vkContext->surfaces.HEIGHT, "Flow Engine");

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window!");

    return SUCCESS;
  }
}