// #include "instances.hpp"
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