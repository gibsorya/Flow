#include "instances.hpp"
#include "../root.hpp"

namespace flow::vulkan
{
    // void startFlow(){
    //     root->flowInstances->instance = createInstance();
    //     debugtools::setupDebugMessenger();
    // }

    VkInstance createInstance()
    {
        if(root->validLayers->enabledValidationLayers && !debugtools::checkValidationLayerSupport()){
            throw std::runtime_error("Validation layers requested, but none are available!");
        }

        VkInstance flowInstance;
        
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Flow";
        appInfo.applicationVersion = VK_VERSION_1_2;
        appInfo.pEngineName = "Flow";
        appInfo.engineVersion = VK_VERSION_1_2;
        appInfo.apiVersion = VK_API_VERSION_1_2;

        auto extensions = debugtools::getRequiredExtensions();

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.pNext = nullptr;

        VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
        if (root->validLayers->enabledValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
            createInfo.ppEnabledLayerNames = root->validLayers->layers.data();

            debugtools::populateDebugMessengerCreateInfo(debugInfo);
            createInfo.pNext = &debugInfo;
        }

        if (vkCreateInstance(&createInfo, nullptr, &flowInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }

        return flowInstance;
    }
}

// FlowInstances::FlowInstances(){
//     if(root->validLayers->enabledValidationLayers && !flow::foundation::checkValidationLayerSupport()){
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

//         auto extensions = flow::foundation::getRequiredExtensions();

//         VkInstanceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         createInfo.pApplicationInfo = &appInfo;
//         createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
//         createInfo.ppEnabledExtensionNames = extensions.data();
//         createInfo.enabledLayerCount = 0;
//         createInfo.ppEnabledLayerNames = nullptr;
//         createInfo.pNext = nullptr;

//         VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
//         if (root->validLayers->enabledValidationLayers)
//         {
//             createInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
//             createInfo.ppEnabledLayerNames = root->validLayers->layers.data();

//             flow::foundation::populateDebugUtilsMessengerCreateInfo(debugInfo);
//             createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
//         }

//         if (vkCreateInstance(&createInfo, nullptr, &flowInstance) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create instance!");
//         }

//         instances.push_back(flowInstance);
// }

// const std::vector<VkInstance>& FlowInstances::getInstances() const{
//     return instances;
// }

// std::unique_ptr<FlowInstances> createFlowInstances(){
//     return std::make_unique<FlowInstances>();
// }