#include "engine.hpp"
#include "root.hpp"

#include "vk/instance.cpp"
#include "flow/foundation/debugTools.cpp"
#include <vulkan/vulkan.h>
#include <iostream>

namespace flow {
	void initVulkan()
	{
		initWindow();
		vulkan::startFlow();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(root->flowWindow->getWindow())) {
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		if(root->validLayers->enabledValidationLayers){
			vulkan::debugtools::DestroyDebugUtilsMessengerEXT(root->flowInstance->instance, root->debugUtils->debugMessenger, nullptr);
		}

		vkDestroyInstance(root->flowInstance->instance, nullptr);

		glfwDestroyWindow(root->flowWindow->getWindow());
		glfwTerminate();

		system("pause");
	}

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		root->flowWindow->setWindow(root->flowWindow->createWindow());
	}
}

