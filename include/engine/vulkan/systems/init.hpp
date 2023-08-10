#ifndef VULKAN_INIT_SYSTEM
#define VULKAN_INIT_SYSTEM

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <SDL_vulkan.h>
#include "surface.hpp"
#include "instance.hpp"

namespace flow
{
  struct FlowVkInitializationSystem
  {
    void Initialize(FlowVkSurfaceComponent &surfaceComponent, FlowVkInstanceComponent &instanceComponent);

    private:
    void CreateVkWindow(FlowVkSurfaceComponent &surfaceComponent);

    void CreateVkInstance(FlowVkInstanceComponent &instanceComponent, FlowVkSurfaceComponent &surfaceComponent);

    void CreateVkDebugMessenger(FlowVkInstanceComponent &instanceComponent);

    void CreateVkSurface(FlowVkSurfaceComponent &surfaceComponent, FlowVkInstanceComponent &instanceComponent);
  };

}
#endif // VULKAN_INIT_SYSTEM
