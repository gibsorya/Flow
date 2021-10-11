#include "surfaces.hpp"

// namespace flow::vulkan
// {
//     VkSurfaceKHR createSurface()
//     {
//         VkSurfaceKHR surface;

//         if (glfwCreateWindowSurface(root.flowInstances.instances.at(0), root.flowWindow.window, nullptr, &surface) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create window surface!");
//         }

//         return surface;
//     }

// }

namespace flow::vulkan::surfaces {

    Error initWindow(GLFWwindow* &window, u32 WIDTH, u32 HEIGHT, const char *title) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);    

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
    
}

