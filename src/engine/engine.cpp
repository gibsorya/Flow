#include "engine.hpp"
#include "root.hpp"

#include "vk/instances.cpp"

#include <vulkan/vulkan.hpp>
#include <iostream>

Root root;

namespace flow
{
	//This method was here to make sure CMake was working, and I'm too lazy to delete it
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
		root.flowSwaps.swapchains.push_back(vulkan::createSwapchain());
		vulkan::createImageViews();
		root.flowRenderPasses.renderPasses.push_back(vulkan::createRenderPass());
		root.graphicsPipelines.graphicsPipelines.push_back(vulkan::pipelines::createGraphicsPipeline());
	}

	void mainLoop()
	{
		while(!glfwWindowShouldClose(root.flowWindow.window)){
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		for(auto pipeline : root.graphicsPipelines.graphicsPipelines){
			vkDestroyPipeline(root.flowDevices.devices.at(0), pipeline, nullptr);
		}
		for(auto pipelineLayout : root.graphicsPipelines.pipelineLayouts){
			vkDestroyPipelineLayout(root.flowDevices.devices.at(0), pipelineLayout, nullptr);
		}
		for(auto renderpass : root.flowRenderPasses.renderPasses){
			vkDestroyRenderPass(root.flowDevices.devices.at(0), renderpass, nullptr);
		}
		for(auto imageView : root.imageViews.swapchainImageViews){
			vkDestroyImageView(root.flowDevices.devices.at(0), imageView, nullptr);
		}

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