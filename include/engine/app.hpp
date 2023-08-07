#ifndef FLOW_ENGINE_APP
#define FLOW_ENGINE_APP

#include <iostream>
#include <entt/entt.hpp>
#include <vulkan/systems/init.hpp>

namespace flow {
  void init();
  void run();
  void shutdown(entt::registry &registry, entt::entity &entity);
}

#endif // FLOW_ENGINE_APP
