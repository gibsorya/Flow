#pragma once
#include "flow/flow.hpp"
#include "window.hpp"
#include "vk/instances.hpp"
#include "vk/devices.hpp"
#include "vk/queues.hpp"
// #include "flow/foundation/debugTools.hpp"
// #include "vk/instance.hpp"
// #include "vk/surface.hpp"
// #include "vk/devices/physicalDevice.hpp"
// #include "vk/devices/logicalDevice.hpp"
// #include "vk/devices/queues.hpp"



// /*

// 	Where all the data for the components in this Vulkan implementation come together. Hence "root"

// */

typedef struct Root Root;
struct Root {
    std::unique_ptr<FlowWindow> flowWindow = std::make_unique<FlowWindow>();
    // FlowInstances flowInstances;
    std::unique_ptr<FlowInstances> flowInstances = std::make_unique<FlowInstances>();
    std::unique_ptr<ValidLayers> validLayers = std::make_unique<ValidLayers>();
    std::unique_ptr<DebugUtils> debugUtils = std::make_unique<DebugUtils>();
    std::unique_ptr<FlowDevices> flowDevices = std::make_unique<FlowDevices>();
};
static std::unique_ptr<Root> root = std::make_unique<Root>();
// typedef struct Root Root;
// struct Root {
// 	std::unique_ptr<FlowWindow> flowWindow = std::make_unique<FlowWindow>();
// 	std::unique_ptr<FlowInstance> flowInstance = std::make_unique<FlowInstance>();
// 	// std::unique_ptr<ValidLayers> validLayers = std::make_unique<ValidLayers>();
//     // std::unique_ptr<DebugUtils> debugUtils = std::make_unique<DebugUtils>();
// 	std::unique_ptr<FlowPhysicalDevice> flowPDs = std::make_unique<FlowPhysicalDevice>();
// 	std::unique_ptr<FlowLogicalDevice> flowDevice = std::make_unique<FlowLogicalDevice>();
// 	std::unique_ptr<FlowSurface> flowSurface = std::make_unique<FlowSurface>();
// 	// std::unique_ptr<QueueFamilyIndicies> queue
// };
// global std::unique_ptr<Root> root = std::make_unique<Root>();
