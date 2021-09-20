#ifndef FLOW_ENGINE
#define FLOW_ENGINE

#include "flow/flow.hpp"

namespace flow {

	void initVulkan();
	void mainLoop();
	void cleanup();
	void initWindow();
}

#endif