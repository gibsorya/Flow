#include <app.hpp>

namespace flow {
  global FlowVkContext *flowVk = nullptr;

  Error setup(){
    Error err;
    flowVk = new FlowVkContext();
    
    err = vulkan::initalize(flowVk);
    return SUCCESS;
  }

  void mainLoop() {
    while(!glfwWindowShouldClose(flowVk->surfaces.window)){
      glfwPollEvents();
    }
  }

  void cleanup() {
    glfwDestroyWindow(flowVk->surfaces.window);
    glfwTerminate();
  }
}