// #include "engine.hpp"
// #include "root.hpp"

// #include "vk/instances.cpp"

// #include <vulkan/vulkan.hpp>
// #include <iostream>

// Root root;

// namespace flow
// {
// 	//This method was here to make sure CMake was working, and I'm too lazy to delete it
// 	void test()
// 	{
// 		std::cout << "Vulkan initiated!" << std::endl;
// 	}

// 	void initVulkan()
// 	{
// 		root.flowWindow.initWindow();

// 		root.flowInstances.instances.push_back(vulkan::createInstance());
// 		vulkan::setupDebugMessenger();
// 		root.flowSurfaces.surfaces.push_back(vulkan::createSurface());
// 		root.flowDevices.physicalDevice = vulkan::pickPhysicalDevice();
// 		root.flowDevices.devices.push_back(vulkan::createLogicalDevice());
// 		root.flowSwaps.swapchains.push_back(vulkan::createSwapchain());
// 		vulkan::createImageViews();
// 		root.flowRenderPasses.renderPasses.push_back(vulkan::createRenderPass());
// 		root.graphicsPipelines.graphicsPipelines.push_back(vulkan::pipelines::createGraphicsPipeline());
// 		root.flowFrameBuffers.swapchainFrameBuffers.push_back(vulkan::createFramebuffers());
// 		root.flowCommandPools.commandPools.push_back(vulkan::createCommandPool());
// 		root.flowCommandBuffers.commandBuffers.push_back(vulkan::createCommandBuffers());
// 		vulkan::createSyncObjects();
// 	}

// 	void mainLoop()
// 	{
// 		while (!glfwWindowShouldClose(root.flowWindow.window))
// 		{
// 			glfwPollEvents();
// 			draw();
// 		}

// 		vkDeviceWaitIdle(root.flowDevices.devices.at(0));
// 	}

// 	void cleanup()
// 	{
// 		cleanupSwapchain();

// 		for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++)
// 		{
// 			vkDestroySemaphore(root.flowDevices.devices.at(0), root.flowSyncObjects.renderFinishedSemaphores[i], nullptr);
// 			vkDestroySemaphore(root.flowDevices.devices.at(0), root.flowSyncObjects.imageAvailableSemaphores[i], nullptr);
// 			vkDestroyFence(root.flowDevices.devices.at(0), root.flowSyncObjects.inFlightFences[i], nullptr);
// 		}

// 		for (auto commandPool : root.flowCommandPools.commandPools)
// 		{
// 			vkDestroyCommandPool(root.flowDevices.devices.at(0), commandPool, nullptr);
// 		}

// 		for (VkDevice device : root.flowDevices.devices)
// 		{
// 			vkDestroyDevice(device, nullptr);
// 		}

// 		if (enabledValidationLayers)
// 		{
// 			vulkan::DestroyDebugUtilsMessengerEXT(root.flowInstances.instances.at(0), root.debugUtils.debugMessenger, nullptr);
// 		}

// 		int i = 0;
// 		for (VkSurfaceKHR surface : root.flowSurfaces.surfaces)
// 		{
// 			vkDestroySurfaceKHR(root.flowInstances.instances.at(i), surface, nullptr);
// 			i++;
// 		}

// 		for (VkInstance instance : root.flowInstances.instances)
// 		{
// 			vkDestroyInstance(instance, nullptr);
// 		}

// 		glfwDestroyWindow(root.flowWindow.window);
// 		glfwTerminate();
// 	}

// 	void cleanupSwapchain()
// 	{
// 		for (auto framebuffers : root.flowFrameBuffers.swapchainFrameBuffers)
// 		{
// 			for (auto framebuffer : framebuffers)
// 			{
// 				vkDestroyFramebuffer(root.flowDevices.devices.at(0), framebuffer, nullptr);
// 			}
// 		}

// 		for (auto commandBuffers : root.flowCommandBuffers.commandBuffers)
// 		{
// 			vkFreeCommandBuffers(root.flowDevices.devices.at(0), root.flowCommandPools.commandPools.at(0), static_cast<u32>(commandBuffers.size()), commandBuffers.data());
// 		}

// 		for (auto pipeline : root.graphicsPipelines.graphicsPipelines)
// 		{
// 			vkDestroyPipeline(root.flowDevices.devices.at(0), pipeline, nullptr);
// 		}

// 		for (auto pipelineLayout : root.graphicsPipelines.pipelineLayouts)
// 		{
// 			vkDestroyPipelineLayout(root.flowDevices.devices.at(0), pipelineLayout, nullptr);
// 		}

// 		for (auto renderpass : root.flowRenderPasses.renderPasses)
// 		{
// 			vkDestroyRenderPass(root.flowDevices.devices.at(0), renderpass, nullptr);
// 		}

// 		for (auto imageView : root.imageViews.swapchainImageViews)
// 		{
// 			vkDestroyImageView(root.flowDevices.devices.at(0), imageView, nullptr);
// 		}

// 		for (auto swapchain : root.flowSwaps.swapchains)
// 		{
// 			vkDestroySwapchainKHR(root.flowDevices.devices.at(0), swapchain, nullptr);
// 		}
// 	}

// 	void recreateSwapchain()
// 	{
// 		int width = 0, height = 0;
// 		glfwGetFramebufferSize(root.flowWindow.window, &width, &height);
// 		while (width == 0 || height == 0)
// 		{
// 			glfwGetFramebufferSize(root.flowWindow.window, &width, &height);
// 			glfwWaitEvents();
// 		}

// 		vkDeviceWaitIdle(root.flowDevices.devices.at(0));

// 		cleanupSwapchain();

// 		root.flowSwaps.swapchains.at(0) = vulkan::createSwapchain();
// 		vulkan::createImageViews();
// 		root.flowRenderPasses.renderPasses.at(0) = vulkan::createRenderPass();
// 		root.graphicsPipelines.graphicsPipelines.at(0) = vulkan::pipelines::createGraphicsPipeline();
// 		root.flowFrameBuffers.swapchainFrameBuffers.at(0) = vulkan::createFramebuffers();
// 		root.flowCommandBuffers.commandBuffers.at(0) = vulkan::createCommandBuffers();
// 	}

// 	// void initWindow()
// 	// {
// 	// 	glfwInit();

// 	// 	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
// 	// 	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

// 	// 	root.flowWindow.window = glfwCreateWindow(root.flowWindow.WIDTH, root.flowWindow.HEIGHT, "Flow Engine", nullptr, nullptr);
// 	// 	glfwSetWindowUserPointer(root.flowWindow.window, root.flowWindow.window);
// 	// 	glfwSetFramebufferSizeCallback(root.flowWindow.window, framebufferResizeCallback);
// 	// }

// 	void draw()
// 	{
// 		vkWaitForFences(root.flowDevices.devices.at(0), 1, &root.flowSyncObjects.inFlightFences[root.flowSyncObjects.currentFrame], VK_TRUE, UINT64_MAX);

// 		u32 imageIndex;
// 		VkResult result = vkAcquireNextImageKHR(root.flowDevices.devices.at(0), root.flowSwaps.swapchains.at(0), UINT64_MAX, root.flowSyncObjects.imageAvailableSemaphores[root.flowSyncObjects.currentFrame], VK_NULL_HANDLE, &imageIndex);

// 		if (result == VK_ERROR_OUT_OF_DATE_KHR)
// 		{
// 			recreateSwapchain();
// 			return;
// 		}
// 		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
// 		{
// 			throw std::runtime_error("Failed to acquire swap chain image!");
// 		}

// 		if (root.flowSyncObjects.imagesInFlight[imageIndex] != VK_NULL_HANDLE)
// 		{
// 			vkWaitForFences(root.flowDevices.devices.at(0), 1, &root.flowSyncObjects.imagesInFlight[root.flowSyncObjects.currentFrame], VK_TRUE, UINT64_MAX);
// 		}

// 		root.flowSyncObjects.imagesInFlight[imageIndex] = root.flowSyncObjects.inFlightFences[root.flowSyncObjects.currentFrame];

// 		VkSubmitInfo submitInfo{};
// 		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

// 		VkSemaphore waitSemaphores[] = {root.flowSyncObjects.imageAvailableSemaphores[root.flowSyncObjects.currentFrame]};
// 		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
// 		submitInfo.waitSemaphoreCount = 1;
// 		submitInfo.pWaitSemaphores = waitSemaphores;
// 		submitInfo.pWaitDstStageMask = waitStages;

// 		submitInfo.commandBufferCount = 1;
// 		submitInfo.pCommandBuffers = &root.flowCommandBuffers.commandBuffers.at(0).at(imageIndex);

// 		VkSemaphore signalSemaphores[] = {root.flowSyncObjects.renderFinishedSemaphores[root.flowSyncObjects.currentFrame]};
// 		submitInfo.signalSemaphoreCount = 1;
// 		submitInfo.pSignalSemaphores = signalSemaphores;

// 		vkResetFences(root.flowDevices.devices.at(0), 1, &root.flowSyncObjects.inFlightFences[root.flowSyncObjects.currentFrame]);

// 		if (vkQueueSubmit(root.flowDevices.graphicsQueue, 1, &submitInfo, root.flowSyncObjects.inFlightFences[root.flowSyncObjects.currentFrame]) != VK_SUCCESS)
// 		{
// 			throw std::runtime_error("Failed to submit draw command buffer!");
// 		}

// 		VkPresentInfoKHR presentInfo{};
// 		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
// 		presentInfo.waitSemaphoreCount = 1;
// 		presentInfo.pWaitSemaphores = signalSemaphores;

// 		VkSwapchainKHR swapchains[] = {root.flowSwaps.swapchains.at(0)};
// 		presentInfo.swapchainCount = 1;
// 		presentInfo.pSwapchains = swapchains;
// 		presentInfo.pImageIndices = &imageIndex;

// 		result = vkQueuePresentKHR(root.flowDevices.presentQueue, &presentInfo);

// 		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || root.flowSyncObjects.framebufferResized)
// 		{
// 			root.flowSyncObjects.framebufferResized = false;
// 			recreateSwapchain();
// 		}
// 		else if (result != VK_SUCCESS)
// 		{
// 			throw std::runtime_error("Failed to present swap chain image!");
// 		}

// 		root.flowSyncObjects.currentFrame = (root.flowSyncObjects.currentFrame + 1) % vulkan::MAX_FRAMES_IN_FLIGHT;
// 	}

// 	// void framebufferResizeCallback(GLFWwindow *window, int width, int height)
// 	// {
// 	// 	auto app = reinterpret_cast<Root *>(glfwGetWindowUserPointer(window));
// 	// 	app->flowSyncObjects.framebufferResized = true;
// 	// }
// }

// void FlowWindow::initWindow()
// {
// 	glfwInit();

// 	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
// 	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

// 	window = glfwCreateWindow(WIDTH, HEIGHT, "Flow Engine", nullptr, nullptr);
// 	glfwSetWindowUserPointer(window, this);
// 	glfwSetFramebufferSizeCallback(window, Root::framebufferResizeCallback);
// }

// void Root::framebufferResizeCallback(GLFWwindow *window, int width, int height)
// {
// 	auto app = reinterpret_cast<Root *>(glfwGetWindowUserPointer(window));
// 	app->flowSyncObjects.framebufferResized = true;
// }