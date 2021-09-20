#ifndef FLOW_INSTANCE
#define FLOW_INSTANCE

#include "../flow/flow.hpp"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

typedef struct FlowInstance FlowInstance;
struct FlowInstance {
	VkInstance instance;
};

namespace flow::vulkan {
	void startFlow();
	void initComponents();
	VkInstance createInstance();
}
#endif // !FLOW_INSTANCE
