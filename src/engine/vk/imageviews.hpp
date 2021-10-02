#ifndef FLOW_IMAGE_VIEWS
#define FLOW_IMAGE_VIEWS

#include "../flow/flow.hpp"
#include <vulkan/vulkan.h>

struct FlowImageViews{
    std::vector<VkImageView> swapchainImageViews;
};

namespace flow::vulkan {
    void createImageViews();
}

#endif