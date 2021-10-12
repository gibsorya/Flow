#ifndef FLOW_SWAPCHAINS
#define FLOW_SWAPCHAINS

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

struct FlowSwapchains
{
    std::vector<vk::SwapchainKHR> swapchains;
    std::vector<vk::Image> swapchainImages;
    std::vector<vk::Format> swapchainImageFormats;
    std::vector<vk::Extent2D> swapchainExtents;
    std::vector<vk::ImageView> swapchainImageViews;
};

struct SwapchainSupportDetails
{
    vk::SurfaceCapabilities2KHR capabilities;
    std::vector<vk::SurfaceFormat2KHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

namespace flow::vulkan
{
    SwapchainSupportDetails querySwapchainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);
    namespace swapchains
    {
        Error createSwapchain(FlowSwapchains &flowSwaps, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, GLFWwindow* window);

        Error createImageViews(FlowSwapchains &flowSwaps, vk::Device device);

        vk::SurfaceFormat2KHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormat2KHR> &availableFormats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
        vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilities2KHR &capabilities, GLFWwindow* window);
    }

}

// struct FlowSwapchains{
//     std::vector<VkSwapchainKHR> swapchains;
//     std::vector<VkImage> swapChainImages;
//     std::vector<VkFormat> swapchainImageFormats;
//     std::vector<VkExtent2D> swapchainExtents;
// };

// struct SwapchainSupportDetails{
//     VkSurfaceCapabilities2KHR capabilities;
//     std::vector<VkSurfaceFormat2KHR> formats;
//     std::vector<VkPresentModeKHR> presentModes;
// };

// namespace flow::vulkan{
//     VkSwapchainKHR createSwapchain();

//     SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);
//     VkSurfaceFormat2KHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormat2KHR>& availableFormats);
//     VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
//     VkExtent2D chooseSwapExtent(VkSurfaceCapabilities2KHR& capabilities);
// }

#endif