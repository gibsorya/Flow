#ifndef FLOW_VULKAN_BUFFER
#define FLOW_VULKAN_BUFFER

#include <vulkan/vulkan.h>

namespace flow {
  struct FlowVkCommandBufferComponent {
    VkCommandBuffer commandBuffer;
  };
}

#endif // FLOW_VULKAN_BUFFER
