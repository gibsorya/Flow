#ifndef FLOW_CONTEXT
#define FLOW_CONTEXT

#include <flow/foundation.hpp>
#include <vulkan/vulkan.h>

#include "components/surfaces.hpp"
#include "components/instances.hpp"
#include "components/devices.hpp"
#include "components/queues.hpp"
#include "components/swapchains.hpp"


struct FlowContext{

    FlowSurfaces flowSurfaces;
    FlowInstances flowInstances;
    FlowDevices flowDevices;
    FlowSwapchains flowSwaps;
};

namespace flow::vulkan {
    Error initialize(FlowContext *flow);

    void mainLoop(FlowContext *flow);

    void cleanup(FlowContext *flow);
}

#endif