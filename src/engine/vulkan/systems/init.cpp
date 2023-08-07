#include "init.hpp"

namespace flow {
  void FlowVkInitializationSystem::Initialize(FlowVkSurfaceComponent &surfaceComponent) {
    CreateVkSurface(surfaceComponent);
  }

  void FlowVkInitializationSystem::CreateVkSurface(FlowVkSurfaceComponent &surfaceComponent) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    surfaceComponent.window = SDL_CreateWindow(
      "Flow",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      window_flags
    );
  }
}
