#pragma once
#include "flow/flow.hpp"
#include "vk/instance.hpp"
#include "window.hpp"

typedef struct Root Root;
struct Root {
	std::unique_ptr<FlowWindow> flowWindow = std::make_unique<FlowWindow>();
	std::unique_ptr<FlowInstance> flowInstance = std::make_unique<FlowInstance>();
};
static std::unique_ptr<Root> root = std::make_unique<Root>();

