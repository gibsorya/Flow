#ifndef FLOW_VULKAN_INSTANCE
#define FLOW_VULKAN_INSTANCE

#include <vulkan/vulkan.h>

namespace flow {
  struct FlowVkInstance {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
  };
}

#endif // FLOW_VULKAN_INSTANCE