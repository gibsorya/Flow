#ifndef FLOW_APP
#define FLOW_APP

#include <flow/foundation.hpp>
#include <engine/vk/flow_context.hpp>
#include <vulkan/vulkan.h>

namespace flow {
    Error setup();
    void cleanup();
}


#endif