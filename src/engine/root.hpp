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
    FlowDevices flowDevices;
};
extern struct Root root;