#include "engine.hpp"
#include "root.hpp"

#include "vk/instances.cpp"

#include <vulkan/vulkan.hpp>
#include <iostream>

Root root;

namespace flow
{
	void test()
	{
		std::cout << "Vulkan initiated!" << std::endl;
	}

	void initVulkan()
	{
		initWindow();
		root.flowInstances.instances.push_back(vulkan::createInstance());
		vulkan::setupDebugMessenger();
		root.flowSurfaces.surfaces.push_back(vulkan::createSurface());
		// root->flowSurfaces->createSurface();
		// root->flowDevices->pickPhysicalDevice();
		// root->flowDevices->createLogicalDevice();
	}

	void mainLoop()
	{
		while(!glfwWindowShouldClose(root.flowWindow.window)){
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		// vkDestroyDevice(root->flowDevices->getDevices().at(0), nullptr);

		if(enabledValidationLayers){
			vulkan::DestroyDebugUtilsMessengerEXT(root.flowInstances.instances.at(0), root.debugUtils.debugMessenger, nullptr);
		}

		// vkDestroySurfaceKHR(root->flowInstances->getInstance(), root->flowSurfaces->getSurfaces().at(0), nullptr);
		// vkDestroyInstance(root->flowInstances->getInstance(), nullptr);

		// // for(VkInstance instance : root->flowInstances->getInstances()){
		// // 	vkDestroyInstance(instance, nullptr);
		// // }

		int i = 0;
		for(VkSurfaceKHR surface : root.flowSurfaces.surfaces){
			vkDestroySurfaceKHR(root.flowInstances.instances.at(i), surface, nullptr);
			i++;
		}

		for(VkInstance instance : root.flowInstances.instances){
			std::cout << "Loop!" << std::endl;
			vkDestroyInstance(instance, nullptr);
		}

		// for(size_t i = 0; i < root->flowInstances.instances.size(); i++){
		// 	std::cout << "Loop!" << std::endl;
		// 	vkDestroyInstance(root->flowInstances.instances.at(i), nullptr);
		// }
		// vkDestroyInstance(root.flowInstances.instance, nullptr);

		glfwDestroyWindow(root.flowWindow.window);
		glfwTerminate();
	}

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// root->flowWindow = (FlowWindow*)malloc(sizeof(FlowWindow));

		root.flowWindow.window = glfwCreateWindow(root.flowWindow.WIDTH, root.flowWindow.HEIGHT, "Flow Engine", nullptr, nullptr);
		// // root->flowWindow->setWindow(root->flowWindow->createWindow());
		// root->flowWindow->window = glfwCreateWindow(root->flowWindow->WIDTH, root->flowWindow->HEIGHT, "Flow Engine", nullptr, nullptr);
	}
}