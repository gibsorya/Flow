#ifndef FLOW_SWAPCHAINS
#define FLOW_SWAPCHAINS

#include "../flow/flow.hpp"
#include <vulkan/vulkan.h>

struct FlowSwapchains{
    std::vector<VkSwapchainKHR> swapchains;
    std::vector<VkImage> swapChainImages;
    std::vector<VkFormat> swapchainImageFormats;
    std::vector<VkExtent2D> swapchainExtents;
};

struct SwapchainSupportDetails{
    VkSurfaceCapabilities2KHR capabilities;
    std::vector<VkSurfaceFormat2KHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace flow::vulkan{
    VkSwapchainKHR createSwapchain();

    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);
    VkSurfaceFormat2KHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormat2KHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(VkSurfaceCapabilities2KHR& capabilities);
}

#endif