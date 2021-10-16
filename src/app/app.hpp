#ifndef FLOW_APP
#define FLOW_APP

#include <flow/foundation.hpp>
#include <engine/vk/flow_context.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace flow {
    Error setup();
    void mainLoop();
    void cleanup();
    void cleanupSwapchain();
    void recreateSwapchain();
    void draw();

    void framebufferResizeCallback(GLFWwindow *window, int width, int height);
}


#endif