#pragma once
#include "flow/flow.hpp"
#include "window.hpp"
#include "vk/instances.hpp"
#include "vk/devices.hpp"
#include "vk/queues.hpp"
#include "vk/surfaces.hpp"


/*

Where all the data for the components in this Vulkan implementation come together. Hence "root"

*/
struct Root
{
    FlowWindow flowWindow;
    FlowInstances flowInstances;
    DebugUtils debugUtils;
    FlowSurfaces flowSurfaces;
    // std::unique_ptr<FlowInstances> flowInstances = std::make_unique<FlowInstances>();
    // std::unique_ptr<FlowWindow> flowWindow = std::make_unique<FlowWindow>();
    // std::unique_ptr<flow::FlowInstances> flowInstances = std::make_unique<flow::FlowInstances>();
    // std::unique_ptr<DebugUtils> debugUtils = std::make_unique<DebugUtils>();
    // std::unique_ptr<FlowDevices> flowDevices = std::make_unique<FlowDevices>();
    // std::unique_ptr<FlowSurfaces> flowSurfaces = std::make_unique<FlowSurfaces>();
};
extern struct Root root;