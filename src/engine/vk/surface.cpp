// #include "surface.hpp"
// #include "../root.hpp"

// namespace flow::vulkan {
//     void createSurface(){
//         if(glfwCreateWindowSurface(root->flowInstance->instances.at(0), root->flowWindow->getWindow(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&root->flowSurface->surface)) != VK_SUCCESS){
//             throw std::runtime_error("Failed to create window surface!");
//         }
//     }
// }