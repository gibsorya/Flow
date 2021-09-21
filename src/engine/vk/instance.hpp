#ifndef FLOW_INSTANCE
#define FLOW_INSTANCE

#include "../flow/flow.hpp"


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

/*

Vulkan instance for Flow.

*/
typedef struct FlowInstance FlowInstance;
struct FlowInstance {
	VkInstance instance;

	std::vector<const char *> extensions;

	VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
};

namespace flow::vulkan {
	void startFlow();
	void initComponents();
	VkInstance createInstance();
}
#endif // !FLOW_INSTANCE
