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

		vk::Instance flowInstance;
		auto extensions = debugtools::getRequiredExtensions();

		auto appInfo = vk::ApplicationInfo("Flow", VK_VERSION_1_2, "Flow Engine", VK_VERSION_1_2, VK_API_VERSION_1_2);
		auto createInfo = vk::InstanceCreateInfo({}, &appInfo, 0, nullptr, static_cast<u32>(extensions.size()), extensions.data());
		
		auto debugInfo = vk::DebugUtilsMessengerCreateInfoEXT();

		if(root->validLayers->enabledValidationLayers){
			createInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
			createInfo.ppEnabledLayerNames = root->validLayers->layers.data();

			debugtools::populateDebugUtilsMessengerCreateInfo(debugInfo);
			createInfo.pNext = &debugInfo;
		} else {
			createInfo.pNext = nullptr;
			createInfo.enabledLayerCount = 0;
		}

		

		if (vk::createInstance(&createInfo, nullptr, &flowInstance) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create instance!");
		}

		return flowInstance;
	}

}