#include "instances.hpp"
// #include "../root.hpp"

// namespace flow::vulkan
// {

//     VkInstance createInstance()
//     {
//         if (enabledValidationLayers && !checkValidationLayerSupport())
//         {
//             throw std::runtime_error("Validation layers requested, but none are available!");
//         }

//         VkInstance flowInstance;

//         VkApplicationInfo appInfo{};
//         appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//         appInfo.pApplicationName = "Flow";
//         appInfo.applicationVersion = VK_VERSION_1_2;
//         appInfo.pEngineName = "Flow";
//         appInfo.engineVersion = VK_VERSION_1_2;
//         appInfo.apiVersion = VK_API_VERSION_1_2;

//         auto extensions = getRequiredExtensions();

//         VkInstanceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         createInfo.pApplicationInfo = &appInfo;
//         createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
//         createInfo.ppEnabledExtensionNames = extensions.data();
//         createInfo.enabledLayerCount = 0;
//         createInfo.ppEnabledLayerNames = nullptr;
//         createInfo.pNext = nullptr;

//         VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
//         if (enabledValidationLayers)
//         {
//             createInfo.enabledLayerCount = static_cast<u32>(validlayers::layers.size());
//             createInfo.ppEnabledLayerNames = validlayers::layers.data();

//             populateDebugMessengerCreateInfo(debugInfo);
//             createInfo.pNext = &debugInfo;
//         }

//         if (vkCreateInstance(&createInfo, nullptr, &flowInstance) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create instance!");
//         }

//         return flowInstance;
//     }

// }

namespace flow::vulkan::instances
{
    Error createInstance(std::vector<vk::Instance> &instances, const char *appName, const char *engineName, std::vector<const char *> extensions)
    {
        if (enabledValidationLayers && !debug::checkValidationLayerSupport())
        {
            ERROR_FAIL(ERR_INVALID, "Validation Layers enabled, but none are supported!");
        }

        Error err;

        vk::Instance instance;

        auto appInfo = vk::ApplicationInfo(appName, VK_VERSION_1_2, engineName, VK_VERSION_1_2, VK_API_VERSION_1_2);

        auto instanceInfo = vk::InstanceCreateInfo({}, &appInfo, 0, nullptr, static_cast<u32>(extensions.size()), extensions.data());

        vk::DebugUtilsMessengerCreateInfoEXT debugInfo;
        
        if (enabledValidationLayers)
        {
            instanceInfo.enabledLayerCount = static_cast<u32>(layers.size());
            instanceInfo.ppEnabledLayerNames = layers.data();

            populateDebugMessengerCreateInfo(debugInfo);
            instanceInfo.pNext = (vk::DebugUtilsMessengerCreateInfoEXT*)&debugInfo;
        }

        vk::Result result = vk::createInstance(&instanceInfo, nullptr, &instance);

        if(result != vk::Result::eSuccess){
            return ERR_CANT_CREATE;
        }

        // ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to create Vulkan instance!");
        // return ERR_CANT_CREATE;

        instances.push_back(instance);

        return SUCCESS;
    }

    //* Overloaded function to pass your own info for InstanceCreateInfo
    Error createInstance(std::vector<vk::Instance> &instances, const char *appName, const char *engineName, vk::InstanceCreateInfo *instanceInfo, std::vector<const char *> extensions)
    {
        Error err;

        vk::Instance instance;

        auto appInfo = vk::ApplicationInfo(appName, VK_VERSION_1_2, engineName, VK_VERSION_1_2, VK_API_VERSION_1_2);

        if (instanceInfo == nullptr)
        {
            instanceInfo->pApplicationInfo = &appInfo;
            instanceInfo->enabledExtensionCount = static_cast<u32>(extensions.size());
            instanceInfo->ppEnabledExtensionNames = extensions.data();

            if (enabledValidationLayers)
            {
                instanceInfo->enabledLayerCount = static_cast<u32>(layers.size());
                instanceInfo->ppEnabledLayerNames = layers.data();
            }
        }

        vk::Result result = vk::createInstance(instanceInfo, nullptr, &instance);

        ERROR_FAIL_COND(result != vk::Result::eSuccess, ERR_CANT_CREATE, "Failed to create Vulkan instance!");

        instances.push_back(instance);

        return SUCCESS;
    }

    Error findInstanceExtensions(std::vector<const char *> &extensions)
    {
        u32 glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        extensions.push_back(*glfwExtensions);

        if (enabledValidationLayers)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);

        return SUCCESS;
    }

    Error setupDebugMessenger(vk::Instance instance, std::vector<vk::DebugUtilsMessengerEXT> &debugMessengers)
    {
        if(!enabledValidationLayers) return SUCCESS;

        vk::DebugUtilsMessengerEXT debugMessenger;

        vk::DebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);

        if(result != VK_SUCCESS){
            return ERR_CANT_CREATE;
        }

        // ERROR_FAIL_COND(result != VK_SUCCESS, ERR_CANT_CREATE, "Failed to create debug messenger!");

        debugMessengers.push_back(debugMessenger);

        return SUCCESS;
    }

    void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
			{},
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			debug::debugCallback,
			nullptr
		);
    }

    VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT")) };

		if (func != nullptr) {
			return func(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugUtilsMessengerEXT*>(pDebugMessenger));
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

    void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks* pAllocator)
	{
		auto func{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT")) };

		if (func != nullptr) {
			func(instance, debugMessenger, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
		}
	}
}
