#include "instance.hpp"
#include "../root.hpp"



namespace flow::vulkan {
	void startFlow()
	{
		root->flowInstance->instance = createInstance();
		initComponents();
	}

	void initComponents()
	{
		debugtools::setupDebugMessenger();
		devices::pickPhysicalDevices();
	}

	VkInstance createInstance() {
		if(root->validLayers->enabledValidationLayers && !debugtools::checkValidationLayerSupport()){
			throw std::runtime_error("Validation layers requested, but none are available!");
		}

		VkInstance flowInstance;
		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo createInfo{};

		appInfo.pApplicationName = "FLOW";
		appInfo.pEngineName = "Flow Engine";
		appInfo.applicationVersion = VK_VERSION_1_2;
		appInfo.engineVersion = VK_VERSION_1_2;
		appInfo.apiVersion = VK_API_VERSION_1_2;

		
		auto extensions = debugtools::getRequiredExtensions();
		VkDebugUtilsMessengerCreateInfoEXT debugInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		
		if(root->validLayers->enabledValidationLayers){
			createInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
			createInfo.ppEnabledLayerNames = root->validLayers->layers.data();

			debugtools::populateDebugUtilsMessengerCreateInfo(debugInfo);
			createInfo.pNext = &debugInfo;
		} else {
			createInfo.pNext = nullptr;
			createInfo.enabledLayerCount = 0;
		}

		

		if (vkCreateInstance(&createInfo, nullptr, &flowInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create instance!");
		}

		return flowInstance;
	}

}