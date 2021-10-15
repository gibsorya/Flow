#ifndef FLOW_CONTEXT
#define FLOW_CONTEXT

#include <flow/foundation.hpp>
#include <vulkan/vulkan.h>

#include "components/surfaces.hpp"
#include "components/instances.hpp"
#include "components/devices.hpp"
#include "components/queues.hpp"
#include "components/swapchains.hpp"
#include "components/pipelines/pipelines.hpp"
#include "components/pipelines/pipelineinfos.hpp"


struct FlowContext{

    FlowSurfaces flowSurfaces;
    FlowInstances flowInstances;
    FlowDevices flowDevices;
    FlowSwapchains flowSwaps;
    FlowGraphics flowGraphics;
};

namespace flow::vulkan {
    Error initialize(FlowContext *flow);

    // void mainLoop(FlowContext *flow);

    // void cleanup(FlowContext *flow);

    void populateGraphicsPipelineData(GraphicsPipelineData &data);
}

#endif