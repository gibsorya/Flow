#ifndef FLOW_INSTANCE
#define FLOW_INSTANCE

#include "../flow/flow.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

typedef struct FlowInstance FlowInstance;
struct FlowInstance {
	VkInstance instance;

	static VkInstance createInstance() {
		VkInstance flowInstance;
		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo createInfo{};

		appInfo.pApplicationName = "FLOW";
		appInfo.pEngineName = "Flow Engine";
		appInfo.applicationVersion = VK_VERSION_1_2;
		appInfo.engineVersion = VK_VERSION_1_2;
		appInfo.apiVersion = VK_API_VERSION_1_2;

		u32 glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.pApplicationInfo = &appInfo;

		if (vkCreateInstance(&createInfo, nullptr, &flowInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create instance!");
		}

		return flowInstance;
	}
};

namespace flow::vulkan {
	void startFlow();
	void initComponents();
}
#endif // !FLOW_INSTANCE
