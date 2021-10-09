// #include "imageviews.hpp"
// #include "../root.hpp"

// namespace flow::vulkan {

//     void createImageViews() {
//         root.imageViews.swapchainImageViews.resize(root.flowSwaps.swapChainImages.size());

//         for(size_t i = 0; i < root.flowSwaps.swapChainImages.size(); i++){
//             VkImageViewCreateInfo createInfo{};
//             createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//             createInfo.image = root.flowSwaps.swapChainImages.at(i);
//             createInfo.format = root.flowSwaps.swapchainImageFormats.at(0);
//             createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
//             createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//             createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//             createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//             createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//             createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             createInfo.subresourceRange.baseMipLevel = 0;
//             createInfo.subresourceRange.levelCount = 1;
//             createInfo.subresourceRange.baseArrayLayer = 0;
//             createInfo.subresourceRange.layerCount = 1;

//             if(vkCreateImageView(root.flowDevices.devices.at(0), &createInfo, nullptr, &root.imageViews.swapchainImageViews.at(i))){
//                 throw std::runtime_error("Failed to create image views!");
//             }
//         }        
//     }
// }