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
		root.flowDevices.physicalDevice = vulkan::pickPhysicalDevice();
		root.flowDevices.devices.push_back(vulkan::createLogicalDevice());
		// root.flowSwaps.swapchains.push_back(vulkan::createSwapchain());
	}

	void mainLoop()
	{
		while(!glfwWindowShouldClose(root.flowWindow.window)){
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		for(VkSwapchainKHR swapchain : root.flowSwaps.swapchains){
			vkDestroySwapchainKHR(root.flowDevices.devices.at(0), swapchain, nullptr);
		}

		for(VkDevice device : root.flowDevices.devices){
			vkDestroyDevice(device, nullptr);
		}

		if(enabledValidationLayers){
			vulkan::DestroyDebugUtilsMessengerEXT(root.flowInstances.instances.at(0), root.debugUtils.debugMessenger, nullptr);
		}

		int i = 0;
		for(VkSurfaceKHR surface : root.flowSurfaces.surfaces){
			vkDestroySurfaceKHR(root.flowInstances.instances.at(i), surface, nullptr);
			i++;
		}

		for(VkInstance instance : root.flowInstances.instances){
			vkDestroyInstance(instance, nullptr);
		}

		glfwDestroyWindow(root.flowWindow.window);
		glfwTerminate();
	}

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		root.flowWindow.window = glfwCreateWindow(root.flowWindow.WIDTH, root.flowWindow.HEIGHT, "Flow Engine", nullptr, nullptr);
	}
}