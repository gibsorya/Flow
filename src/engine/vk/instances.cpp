#include "instances.hpp"
#include "../root.hpp"

namespace flow::vulkan
{

    VkInstance createInstance()
    {
        if (enabledValidationLayers && !checkValidationLayerSupport())
        {
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

        auto extensions = getRequiredExtensions();

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.pNext = nullptr;

        VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
        if (enabledValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<u32>(validlayers::layers.size());
            createInfo.ppEnabledLayerNames = validlayers::layers.data();

            populateDebugMessengerCreateInfo(debugInfo);
            createInfo.pNext = &debugInfo;
        }

        if (vkCreateInstance(&createInfo, nullptr, &flowInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }

        // root->flowInstances->instance = flowInstance;
        return flowInstance;
    }

}
//     void createInstance()
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

//         // root->flowInstances->instance = flowInstance;
//     }
// }

// }

// const VkInstance &FlowInstances::findInstance() const
// {
//     if (root->flowInstances->getInstances().at(0) != VK_NULL_HANDLE)
//     {
//         return root->flowInstances->getInstances().at(0);
//     }
//     else
//     {
//         return root->flowInstances->getInstance();
//     }
// }

// const std::vector<VkInstance> &FlowInstances::getInstances() const
// {
//     return instances;
// }

// namespace flow
// {
// void FlowInstances::createInstance()
// {

//     if (flow::enabledValidationLayers && !flow::vulkan::checkValidationLayerSupport())
//     {
//         throw std::runtime_error("Validation layers requested, but none are available!");
//     }

//     // flowInstance = std::make_shared<VkInstance>();

//     VkApplicationInfo appInfo{};
//     appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//     appInfo.pApplicationName = "Flow";
//     appInfo.applicationVersion = VK_VERSION_1_2;
//     appInfo.pEngineName = "Flow";
//     appInfo.engineVersion = VK_VERSION_1_2;
//     appInfo.apiVersion = VK_API_VERSION_1_2;

//     auto extensions = flow::vulkan::getRequiredExtensions();

//     VkInstanceCreateInfo createInfo{};
//     createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//     createInfo.pApplicationInfo = &appInfo;
//     createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
//     createInfo.ppEnabledExtensionNames = extensions.data();
//     createInfo.enabledLayerCount = 0;
//     createInfo.ppEnabledLayerNames = nullptr;
//     createInfo.pNext = nullptr;

//     VkDebugUtilsMessengerCreateInfoEXT debugInfo{};

//     vk::ApplicationInfo appInfo2("Hello Triangle", VK_VERSION_1_2, "Vulkan Engine", VK_VERSION_1_2, VK_API_VERSION_1_2);

//     vk::InstanceCreateInfo createInfo2({}, &appInfo2, 0, nullptr, static_cast<uint32_t>(extensions.size()), extensions.data());

//     if (flow::enabledValidationLayers)
//     {
//         createInfo2.enabledLayerCount = static_cast<uint32_t>(flow::validlayers::layers.size());
//         createInfo2.ppEnabledLayerNames = flow::validlayers::layers.data();

//         flow::vulkan::populateDebugMessengerCreateInfo(debugInfo);
//         createInfo2.pNext = reinterpret_cast<vk::DebugUtilsMessengerCreateInfoEXT *>(&debugInfo);
//     }

//     if (flow::enabledValidationLayers)
//     {
//         createInfo.enabledLayerCount = static_cast<u32>(flow::validlayers::layers.size());
//         createInfo.ppEnabledLayerNames = flow::validlayers::layers.data();

//         flow::vulkan::populateDebugMessengerCreateInfo(debugInfo);
//         createInfo.pNext = &debugInfo;
//     }

//     if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to create instance!");
//     }

//     // if (vk::createInstance(&createInfo2, nullptr, &vkInstance) != vk::Result::eSuccess)
//     // {
//     //     throw std::runtime_error("Failed to create instance! :(");
//     // }

//     this->instances.push_back(this->instance);
// }

// const VkInstance &FlowInstances::getInstance() const
// {
//     return instance;
// }

// VkInstance FlowInstances::createInstances() {
//     if (root->validLayers->enabledValidationLayers && !flow::vulkan::checkValidationLayerSupport())
//     {
//         throw std::runtime_error("Validation layers requested, but none are available!");
//     }

//     VkApplicationInfo appInfo{};
//     appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//     appInfo.pApplicationName = "Flow";
//     appInfo.applicationVersion = VK_VERSION_1_2;
//     appInfo.pEngineName = "Flow";
//     appInfo.engineVersion = VK_VERSION_1_2;
//     appInfo.apiVersion = VK_API_VERSION_1_2;

//     auto extensions = flow::vulkan::getRequiredExtensions();

//     VkInstanceCreateInfo createInfo{};
//     createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//     createInfo.pApplicationInfo = &appInfo;
//     createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
//     createInfo.ppEnabledExtensionNames = extensions.data();
//     createInfo.enabledLayerCount = 0;
//     createInfo.ppEnabledLayerNames = nullptr;
//     createInfo.pNext = nullptr;

//     VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
//     if (root->validLayers->enabledValidationLayers)
//     {
//         createInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
//         createInfo.ppEnabledLayerNames = root->validLayers->layers.data();

//         flow::vulkan::populateDebugMessengerCreateInfo(debugInfo);
//         createInfo.pNext = &debugInfo;
//     }

//     if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to create instance!");
//     }

//     instances.push_back(this->instance);

//     return instance;
// }

// FlowInstances::FlowInstances(){
//     instance = createInstances();
// }

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

// }