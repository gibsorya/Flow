#ifndef FLOW_VULKAN_SWAPCHAIN
#define FLOW_VULKAN_SWAPCHAIN

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace flow {
  struct FlowVkSwapchainComponent {
    VkSwapchainKHR swapchain;
    VkSwapchainCreateInfoKHR createInfo;

    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    // std::
  };

  struct SwapchainSupportDetails {
    VkSurfaceCapabilities2KHR capabilities;
    std::vector<VkSurfaceFormat2KHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
}

#endif // FLOW_VULKAN_SWAPCHAIN
