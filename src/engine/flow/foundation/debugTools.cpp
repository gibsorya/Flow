// #include "debugTools.hpp"
// #include "../../root.hpp"

// #include <iostream>

// namespace flow::vulkan::debugtools
// {
//     bool checkValidationLayerSupport()
//     {
//         uint32_t layerCount;
//         vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

//         std::vector<VkLayerProperties> availableLayers(layerCount);
//         vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

//         for (const char *layerName : root->validLayers->layers)
//         {
//             bool layerFound = false;

//             for (const auto &layerProperties : availableLayers)
//             {
//                 if (strcmp(layerName, layerProperties.layerName) == 0)
//                 {
//                     layerFound = true;
//                     break;
//                 }
//             }

//             if (!layerFound)
//             {
//                 return false;
//             }
//         }

//         return true;
//     }
//     std::vector<const char *> getRequiredExtensions()
//     {
//         u32 glfwExtensionCount = 0;
//         const char **glfwExtensions;
//         glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

//         std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

//         if (root->validLayers->enabledValidationLayers)
//         {
//             extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//         }

//         return extensions;
//     }

//     void populateDebugUtilsMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo)
//     {
//         createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
//             {},
//             vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
//             vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
//             debugCallback,
//             nullptr);
//     }

//     static VkBool32 VKAPI_PTR VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
//     {
//         std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
//         return VK_FALSE;
//     }

//     void setupDebugMessenger()
//     {
//         if (!root->validLayers->enabledValidationLayers)
//             return;
//         vk::DebugUtilsMessengerCreateInfoEXT createInfo;
//         populateDebugUtilsMessengerCreateInfo(createInfo);

//         if (CreateDebugUtilsMessengerEXT(root->flowInstance->instances.at(0), &createInfo, nullptr, &root->debugUtils->debugMessenger))
//         {
//             throw std::runtime_error("Failed to setup debug messenger!");
//         }
//     }

//     VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo, const vk::AllocationCallbacks *pAllocator, vk::DebugUtilsMessengerEXT *pDebugMessenger)
//     {
//         auto func{reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"))};

//         if (func != nullptr)
//         {
//             return func(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks *>(pAllocator), reinterpret_cast<VkDebugUtilsMessengerEXT *>(pDebugMessenger));
//         }
//         else
//         {
//             return VK_ERROR_EXTENSION_NOT_PRESENT;
//         }
//     }

//     void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks *pAllocator)
//     {
//         auto func{reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"))};

//         if (func != nullptr)
//         {
//             func(instance, debugMessenger, reinterpret_cast<const VkAllocationCallbacks *>(pAllocator));
//         }
//     }
// }