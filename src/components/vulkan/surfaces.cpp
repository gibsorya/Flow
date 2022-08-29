#include <surfaces.hpp>

namespace flow::vulkan::surfaces {
  Error initWindow(GLFWwindow *&window, u32 WIDTH, u32 HEIGHT, const char *title) {
    std::cout << "INIT WINDOW" << std::endl;
    glfwInit();

    std::cout << "INIT WINDOW" << std::endl;
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    
    return SUCCESS;
  }
}