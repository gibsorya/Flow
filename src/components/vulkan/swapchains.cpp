#include <swapchains.hpp>
#include <queues.hpp>

namespace flow::vulkan
{
  SwapchainSupportDetails querySwapchainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface)
  {
    SwapchainSupportDetails details;

    auto physicalSurfaceInfo = vk::PhysicalDeviceSurfaceInfo2KHR();
    physicalSurfaceInfo.surface = surface;

    u32 formatCount;
    vk::Result result = device.getSurfaceCapabilities2KHR(&physicalSurfaceInfo, &details.capabilities);
    result = device.getSurfaceFormats2KHR(&physicalSurfaceInfo, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      result = device.getSurfaceFormats2KHR(&physicalSurfaceInfo, &formatCount, details.formats.data());
    }

    u32 presentModeCount;
    result = device.getSurfacePresentModesKHR(surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
      details.presentModes.resize(presentModeCount);
      result = device.getSurfacePresentModesKHR(surface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }
  namespace swapchains
  {
    Error createSwapchain(vk::SwapchainKHR &swapchain, vk::Extent2D &swapExtent, std::vector<vk::Image> &swapchainImages, vk::Format &imageFormat, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, GLFWwindow *window)
    {
      QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
      u32 queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

      SwapchainSupportDetails details = querySwapchainSupport(physicalDevice, surface);

      vk::SurfaceFormat2KHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
      vk::PresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
      vk::Extent2D extent = chooseSwapExtent(details.capabilities, window);

      u32 imageCount = details.capabilities.surfaceCapabilities.minImageCount + 1;

      vk::SwapchainCreateInfoKHR createInfo{};
      createInfo.surface = surface;
      createInfo.minImageCount = imageCount;
      createInfo.imageExtent = extent;
      createInfo.imageFormat = surfaceFormat.surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.surfaceFormat.colorSpace;
      createInfo.imageArrayLayers = 1; // Specifies the amount of layers each image consists of. This is always 1 unless you are developing a stereoscopic 3D application
      createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

      if (indices.graphicsFamily != indices.presentFamily)
      {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
      }
      else
      {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
      }

      createInfo.presentMode = presentMode;
      createInfo.preTransform = details.capabilities.surfaceCapabilities.currentTransform;
      createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
      createInfo.clipped = VK_TRUE;
      createInfo.oldSwapchain = VK_NULL_HANDLE;

      if (device.createSwapchainKHR(&createInfo, nullptr, &swapchain) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }

      vk::Result result = device.getSwapchainImagesKHR(swapchain, &imageCount, nullptr);
      swapchainImages.resize(imageCount);
      result = device.getSwapchainImagesKHR(swapchain, &imageCount, swapchainImages.data());

      imageFormat = surfaceFormat.surfaceFormat.format;
      swapExtent = extent;

      return SUCCESS;
    }

    vk::SurfaceFormat2KHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormat2KHR> &availableFormats)
    {
      for (const auto &availableFormat : availableFormats)
      {
        if (availableFormat.surfaceFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
          return availableFormat;
        }
      }

      return availableFormats.at(0);
    }

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
    {
      for (const auto &availablePresentMode : availablePresentModes)
      {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
          return availablePresentMode;
        }
      }

      return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilities2KHR &capabilities, GLFWwindow *window)
    {
      if (capabilities.surfaceCapabilities.currentExtent.width != UINT32_MAX)
      {
        return capabilities.surfaceCapabilities.currentExtent;
      }
      else
      {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        vk::Extent2D actualExtent = {
            static_cast<u32>(width),
            static_cast<u32>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.surfaceCapabilities.minImageExtent.width, capabilities.surfaceCapabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.surfaceCapabilities.minImageExtent.height, capabilities.surfaceCapabilities.maxImageExtent.height);

        return actualExtent;
      }
    }

    Error createImageViews(FlowVkSwapchains &flowSwaps, vk::Device device)
    {
      flowSwaps.swapchainImageViews.resize(flowSwaps.swapchainImages.size());

      for (size_t i = 0; i < flowSwaps.swapchainImages.size(); i++)
      {
        auto createInfo = vk::ImageViewCreateInfo(
            {},                                    /* flags */
            flowSwaps.swapchainImages.at(i),       /* vk::Image image */
            vk::ImageViewType::e2D,                /* Image View Type */
            flowSwaps.swapchainImageFormats.at(0), /* Image Format */
            {
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity}, /* Components */
            {
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1});

        if (device.createImageView(&createInfo, nullptr, &flowSwaps.swapchainImageViews.at(i)) != vk::Result::eSuccess)
        {
          return ERR_CANT_CREATE;
        }
      }

      return SUCCESS;
    }
  }
}