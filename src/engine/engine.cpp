#include "engine.hpp"
#include "root.hpp"

#include "vk/instances.cpp"

#include <vulkan/vulkan.hpp>
#include <iostream>

namespace flow {
	void test(){
		std::cout << "Vulkan initiated!" << std::endl;
	}

	void initVulkan(){
		initWindow();
		root->flowInstances->instance = vulkan::createInstance();
		// root->flowInstances = createFlowInstances();
		root->debugUtils->debugMessenger = vulkan::debugtools::setupDebugMessenger(&root->flowInstances->instance);
		vulkan::pickPhysicalDevice(&root->flowInstances->instance);
		root->flowDevices->device = vulkan::createLogicalDevice(&root->flowDevices->currentPhysicalDevice);
	}

	void mainLoop(){
		while(!glfwWindowShouldClose(root->flowWindow->window)){
			glfwPollEvents();
		}
	}

	void cleanup(){
		vkDestroyDevice(root->flowDevices->device, nullptr);

		if(root->validLayers->enabledValidationLayers){
			vulkan::debugtools::DestroyDebugUtilsMessengerEXT(root->flowInstances->instance, root->debugUtils->debugMessenger, nullptr);
		}

		vkDestroyInstance(root->flowInstances->instance, nullptr);

		// for(VkInstance instance : root->flowInstances->getInstances()){
		// 	vkDestroyInstance(instance, nullptr);
		// }

		glfwDestroyWindow(root->flowWindow->window);
		glfwTerminate();
	}

	void initWindow(){
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// root->flowWindow->setWindow(root->flowWindow->createWindow());
		root->flowWindow->window = glfwCreateWindow(root->flowWindow->WIDTH, root->flowWindow->HEIGHT, "Flow Engine", nullptr, nullptr);
	}
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


