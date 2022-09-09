#ifndef FLOW_VK_SWAPCHAINS
#define FLOW_VK_SWAPCHAINS

#include <foundation.hpp>
#include <GLFW/glfw3.h>

struct FlowVkSwapchains
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
    Error createSwapchain(vk::SwapchainKHR &swapchain, vk::Extent2D &swapExtent, std::vector<vk::Image> &swapchainImages, vk::Format &imageFormat, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, GLFWwindow *window);

    Error createImageViews(FlowVkSwapchains &flowSwaps, vk::Device device);

    vk::SurfaceFormat2KHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormat2KHR> &availableFormats);
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
    vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilities2KHR &capabilities, GLFWwindow *window);
  }
}

#endif