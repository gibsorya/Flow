#pragma once
#include "flow/flow.hpp"
#include "flow/foundation/debugTools.hpp"
#include "vk/instance.hpp"
#include "vk/surface.hpp"
#include "vk/devices/physicalDevice.hpp"
#include "vk/devices/logicalDevice.hpp"
#include "vk/devices/queues.hpp"
#include "window.hpp"


/*

	Where all the data for the components in this Vulkan implementation come together. Hence "root"

*/
typedef struct Root Root;
struct Root {
	std::unique_ptr<FlowWindow> flowWindow = std::make_unique<FlowWindow>();
	std::unique_ptr<FlowInstance> flowInstance = std::make_unique<FlowInstance>();
	std::unique_ptr<ValidLayers> validLayers = std::make_unique<ValidLayers>();
    std::unique_ptr<DebugUtils> debugUtils = std::make_unique<DebugUtils>();
	std::unique_ptr<FlowPhysicalDevice> flowPDs = std::make_unique<FlowPhysicalDevice>();
	std::unique_ptr<FlowLogicalDevice> flowDevice = std::make_unique<FlowLogicalDevice>();
	std::unique_ptr<FlowSurface> flowSurface = std::make_unique<FlowSurface>();
	// std::unique_ptr<QueueFamilyIndicies> queue
};
global std::unique_ptr<Root> root = std::make_unique<Root>();
