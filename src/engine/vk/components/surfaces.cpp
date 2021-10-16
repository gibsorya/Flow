#include "surfaces.hpp"
#include <engine/vk/flow_context.hpp>
#include <app/app.hpp>

namespace flow::vulkan::surfaces {

    Error initWindow(GLFWwindow* &window, u32 WIDTH, u32 HEIGHT, const char *title, FlowSurfaces *surface) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
        glfwSetWindowUserPointer(window, &surface->window);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);  

        return SUCCESS;
    }

    Error createSurface(std::vector<vk::SurfaceKHR> &surfaces, vk::Instance instance, GLFWwindow* window) {
        vk::SurfaceKHR surface;
        
        if(glfwCreateWindowSurface(instance, window, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)) != VK_SUCCESS){
            return ERR_CANT_CREATE;
        }

        surfaces.push_back(surface);
        return SUCCESS;
    }

    // global void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    //     flow->
        
    
    // }
    
}

