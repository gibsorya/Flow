#include "swapchains.hpp"
#include "../root.hpp"

namespace flow::vulkan{

    VkSwapchainKHR createSwapchain() {
        VkSwapchainKHR swapchain;
        
        QueueFamilyIndicies indices = findQueueFamilies(root.flowDevices.physicalDevice);
        u32 queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        SwapchainSupportDetails details = querySwapchainSupport(root.flowDevices.physicalDevice);

        VkSurfaceFormat2KHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
        VkExtent2D extent = chooseSwapExtent(details.capabilities);

        u32 imageCount = details.capabilities.surfaceCapabilities.minImageCount + 1;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = root.flowSurfaces.surfaces.at(0);
        createInfo.minImageCount = imageCount;
        createInfo.imageExtent = extent;
        createInfo.imageFormat = surfaceFormat.surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.surfaceFormat.colorSpace;
        createInfo.imageArrayLayers = 1; //Specifies the amount of layers each image consists of. This is always 1 unless you are developing a stereoscopic 3D application
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if(indices.graphicsFamily != indices.presentFamily){
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        
        createInfo.presentMode = presentMode;
        createInfo.preTransform = details.capabilities.surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(root.flowDevices.devices.at(0), &createInfo, nullptr, &swapchain) != VK_SUCCESS){
            throw std::runtime_error("Failed to create swapchain!");
        }

        vkGetSwapchainImagesKHR(root.flowDevices.devices.at(0), swapchain, &imageCount, nullptr);
        root.flowSwaps.swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(root.flowDevices.devices.at(0), swapchain, &imageCount, root.flowSwaps.swapChainImages.data());

        root.flowSwaps.swapchainImageFormats.push_back(surfaceFormat.surfaceFormat.format);
        root.flowSwaps.swapchainExtents.push_back(extent);

        return swapchain;
    }

    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device) {
        SwapchainSupportDetails details;

        //TO DO: create this info for each surface
		root.flowSurfaces.physicalSurfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
        root.flowSurfaces.physicalSurfaceInfo.pNext = nullptr;
		root.flowSurfaces.physicalSurfaceInfo.surface = root.flowSurfaces.surfaces.at(0);

		details.capabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
		details.capabilities.pNext = nullptr;

        vkGetPhysicalDeviceSurfaceCapabilities2KHR(device, &root.flowSurfaces.physicalSurfaceInfo, &details.capabilities);

        u32 formatCount;
        vkGetPhysicalDeviceSurfaceFormats2KHR(device, &root.flowSurfaces.physicalSurfaceInfo, &formatCount, nullptr);

        if(formatCount != 0){
            details.formats.resize(formatCount);

            for(VkSurfaceFormat2KHR format : details.formats){
                format.sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR;
                format.pNext = nullptr;
            }

            vkGetPhysicalDeviceSurfaceFormats2KHR(device, &root.flowSurfaces.physicalSurfaceInfo, &formatCount, details.formats.data());
        }

        u32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, root.flowSurfaces.surfaces.at(0), &presentModeCount, nullptr);

        if(presentModeCount != 0){
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, root.flowSurfaces.surfaces.at(0), &presentModeCount, details.presentModes.data());
        }

        return details;    
    }

    VkSurfaceFormat2KHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormat2KHR>& availableFormats) {
        for(const auto& availableFormat : availableFormats){
            if(availableFormat.surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
                return availableFormat;
            }
        }

        return availableFormats.at(0);
        
    
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for(const auto& availablePresentMode : availablePresentModes){
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR){
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(VkSurfaceCapabilities2KHR& capabilities) {
        if(capabilities.surfaceCapabilities.currentExtent.width != UINT32_MAX){
            return capabilities.surfaceCapabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(root.flowWindow.window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<u32>(width),
                static_cast<u32>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.surfaceCapabilities.minImageExtent.width, capabilities.surfaceCapabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.surfaceCapabilities.minImageExtent.height, capabilities.surfaceCapabilities.maxImageExtent.height);

            return actualExtent;
        }
        
    
    }

}