#ifndef FLOW_APP
#define FLOW_APP

#include <flow/foundation.hpp>
#include <vulkan/vulkan.h>

namespace app {
    local Error setup();
    local void cleanup();
}


#endif