#ifndef FLOW_INSTANCE
#define FLOW_INSTANCE

#include "../flow/flow.hpp"


#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

/*

Vulkan instance for Flow.

*/
typedef struct FlowInstance FlowInstance;
struct FlowInstance {
	std::vector<vk::Instance> instances;
	vk::Instance instance;

	std::vector<const char *> extensions;

	vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
};

namespace flow::vulkan {
	void startFlow();
	void initComponents();
	vk::Instance createInstance();
}
#endif // !FLOW_INSTANCE
