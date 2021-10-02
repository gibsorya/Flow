#pragma once
#include "flow/flow.hpp"
#include "window.hpp"
#include "vk/instances.hpp"
#include "vk/devices.hpp"
#include "vk/queues.hpp"
#include "vk/surfaces.hpp"
#include "vk/swapchains.hpp"
#include "vk/imageviews.hpp"

/*

Where all the data for the components in this Vulkan implementation come together. Hence "root"

*/
struct Root
{
    FlowWindow flowWindow;
    FlowInstances flowInstances;
    DebugUtils debugUtils;
    FlowSurfaces flowSurfaces;
    std::unique_ptr<FlowDevices> flowDevices2 = std::make_unique<FlowDevices>();
    FlowDevices flowDevices;
    FlowSwapchains flowSwaps;
    FlowImageViews imageViews;
};
extern struct Root root;