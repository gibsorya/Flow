#include "engine.hpp"
// #include "root.hpp"

// #include "vk/surface.cpp"
// #include "vk/instance.cpp"
// #include "flow/foundation/debugTools.cpp"
// #include "vk/devices/physicalDevice.cpp"
// #include "vk/devices/logicalDevice.cpp"
// #include "vk/devices/queues.cpp"
#include <vulkan/vulkan.hpp>
#include <iostream>

namespace flow {
	void test(){
		std::cout << "Vulkan initiated!" << std::endl;
		system("pause");
	}
	// void initVulkan()
	// {
	// 	initWindow();
	// 	vulkan::startFlow();
	// 	std::cout << "Vulkan initiated!" << std::endl;
	// }

	// void mainLoop()
	// {
	// 	while (!glfwWindowShouldClose(root->flowWindow->getWindow())) {
	// 		glfwPollEvents();
	// 	}
	// }

	// void cleanup()
	// {
	// 	root->flowDevice->device.destroy();

	// 	if(root->validLayers->enabledValidationLayers){
	// 		vulkan::debugtools::DestroyDebugUtilsMessengerEXT(root->flowInstance->instances.at(0), root->debugUtils->debugMessenger, nullptr);
	// 	}

	// 	// root->flowInstance->instance.destroySurfaceKHR(root->flowSurface->surface);
	// 	// root->flowInstance->instance.destroy();
	// 	for(auto instance : root->flowInstance->instances){
	// 		instance.destroySurfaceKHR(root->flowSurface->surface);
	// 		instance.destroy();
	// 	}

	// 	glfwDestroyWindow(root->flowWindow->getWindow());
	// 	glfwTerminate();

	// 	system("pause");
	// }

	// void initWindow()
	// {
	// 	glfwInit();

	// 	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// 	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// 	root->flowWindow->setWindow(root->flowWindow->createWindow());
	// }
}

