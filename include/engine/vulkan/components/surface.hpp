#ifndef FLOW_VULKAN_SURFACE
#define FLOW_VULKAN_SURFACE

#include <vulkan/vulkan.h>
#include "SDL.h"

namespace flow
{
  struct FlowVkSurfaceComponent {
    VkSurfaceKHR surface;
    SDL_Window* window{nullptr};

    uint32_t width{800};
    uint32_t height{600};
  };
}

#endif // FLOW_VULKAN_SURFACE
