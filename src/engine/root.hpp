#pragma once
#include "flow/flow.hpp"
#include "flow/foundation/debugTools.hpp"
#include "vk/instance.hpp"
#include "window.hpp"

typedef struct Root Root;
struct Root {
	std::unique_ptr<FlowWindow> flowWindow = std::make_unique<FlowWindow>();
	std::unique_ptr<FlowInstance> flowInstance = std::make_unique<FlowInstance>();
	std::unique_ptr<ValidLayers> validLayers = std::make_unique<ValidLayers>();
    std::unique_ptr<DebugUtils> debugUtils = std::make_unique<DebugUtils>();
};
global std::unique_ptr<Root> root = std::make_unique<Root>();
