#ifndef FLOW_VULKAN_SURFACE
#define FLOW_VULKAN_SURFACE

#include <vulkan/vulkan.h>
#include "SDL.h"

namespace flow
{
  struct FlowVkSurfaceComponent {
    VkSurfaceKHR surface;
    SDL_Window* window{nullptr};
  };
}

#endif // FLOW_VULKAN_SURFACE
