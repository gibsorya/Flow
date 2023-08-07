#ifndef VULKAN_INIT_SYSTEM
#define VULKAN_INIT_SYSTEM

#include <vulkan/vulkan.h>
#include "surface.hpp"

namespace flow
{
  struct FlowVkInitializationSystem
  {
    void Initialize(FlowVkSurfaceComponent &surfaceComponent);

    private:
    void CreateVkSurface(FlowVkSurfaceComponent &surfaceComponent);
  };

}
#endif // VULKAN_INIT_SYSTEM
