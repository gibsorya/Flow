#include "app.hpp"

namespace flow {
  void init() {
    entt::registry registry;

    auto entity = registry.create();
    FlowVkSurfaceComponent& surfaceComponent = registry.emplace<FlowVkSurfaceComponent>(entity);
    FlowVkInstanceComponent& instanceComponent = registry.emplace<FlowVkInstanceComponent>(entity);
    FlowVkPhysicalDeviceComponent& physicalDeviceComponent = registry.emplace<FlowVkPhysicalDeviceComponent>(entity);
    FlowVkLogicalDeviceComponent& logicalDeviceComponent = registry.emplace<FlowVkLogicalDeviceComponent>(entity);

    FlowVkInitializationSystem initializationSystem;
    initializationSystem.Initialize(surfaceComponent, instanceComponent, physicalDeviceComponent, logicalDeviceComponent);

    run();

    shutdown(registry, entity);
  }

  void run() {
    SDL_Event event;
    while(true) {
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) {
        break;
      }
    }
  }

  void shutdown(entt::registry &registry, entt::entity &entity) {
    if(registry.storage<FlowVkPhysicalDeviceComponent>().contains(entity)) {
      auto &physicalDeviceComponent = registry.get<FlowVkPhysicalDeviceComponent>(entity);
      // vkDestroyDevice(physicalDeviceComponent.physicalDevice, nullptr);
    }

    if(registry.storage<FlowVkSurfaceComponent>().contains(entity)) {
      auto &instanceComponent = registry.get<FlowVkInstanceComponent>(entity);
      auto &surfaceComponent = registry.get<FlowVkSurfaceComponent>(entity);
      vkDestroySurfaceKHR(instanceComponent.instance, surfaceComponent.surface, nullptr);
    }

    if(registry.storage<FlowVkInstanceComponent>().contains(entity)) {
      auto &instanceComponent = registry.get<FlowVkInstanceComponent>(entity);
      if(enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instanceComponent.instance, instanceComponent.debugMessenger, nullptr);
      }
      vkDestroyInstance(instanceComponent.instance, nullptr);
    }

    if(registry.storage<FlowVkSurfaceComponent>().contains(entity)) {
      auto &surfaceComponent = registry.get<FlowVkSurfaceComponent>(entity);
      SDL_DestroyWindow(surfaceComponent.window);
    }
  }
}
