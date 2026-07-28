#include <iostream>
#include <cstdint>
#include <string.h>
#include <stdio.h>

#include "renderer/backends/vk/engine/instance.h"
#include "renderer/backends/vk/helpers.h"


VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT message_types, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *p_user_data)
{
    // ---- Filtered warnings ----
    if (strcmp(callback_data->pMessageIdName, "WARNING-Shader-OutputNotConsumed") == 0) return VK_FALSE;

    // ---- Print ----
    std::cerr << "---- Vulkan Debug callback (id = " << callback_data->pMessageIdName << ") ----\n";
    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)std::cerr << "[ERROR] ";
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)std::cerr << "[WARNING] ";
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)std::cerr << "[INFO] ";
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)std::cerr << "[VERBOSE] ";

    std::cerr << " validation layer: " << callback_data->pMessage << '\n';
    return VK_FALSE;
}

namespace engine
{
    void createVulkanInstance(const NativeWindowInfo* native, VkInstance &instance)
    {
        std::vector<const char*> extensions = getExtensions(native);
        std::vector<const char*> layers = getLayers();

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Unknown";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName = "Flow Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo instanceInfo;
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pNext = nullptr;
        instanceInfo.pApplicationInfo = &appInfo;
        // instanceInfo.flags = ENABLE FOR MACOS
        instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
        instanceInfo.ppEnabledExtensionNames = extensions.data();
        instanceInfo.enabledLayerCount = (uint32_t)layers.size();
        instanceInfo.ppEnabledLayerNames = layers.data();
        
        checkVkResult(vkCreateInstance(&instanceInfo, nullptr, &instance));
    }

    std::vector<const char*> getExtensions(const NativeWindowInfo* native)
    {
        std::vector<const char*> extensions;
        #ifdef DEBUG_BUILD
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #endif
        uint32_t windowExtCount = 0;
        const char** windowExts = native->getRequiredInstanceExtensions(&windowExtCount);

        for (uint32_t i = 0; i < windowExtCount; i++) {
            extensions.push_back(windowExts[i]);
        }

        return extensions;
    }

    std::vector<const char*> getLayers()
    {
        std::vector<const char*> layers;
        #ifdef DEBUG_BUILD
            const char* validation_layer_name = "VK_LAYER_KHRONOS_validation";
            if(isSupportedLayer(validation_layer_name)) layers.push_back(validation_layer_name);
        #endif

        return layers;
    }

    VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        #ifdef DEBUG_BUILD
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
            // createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        #endif
    
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;

        return createInfo;
    }

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pMessenger)
    {
        auto fn = (PFN_vkCreateDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (!fn) return VK_ERROR_EXTENSION_NOT_PRESENT;
        return fn(instance, pCreateInfo, pAllocator, pMessenger);
    }

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT messenger,
        const VkAllocationCallbacks *pAllocator)
    {
        auto fn = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (fn)
            fn(instance, messenger, pAllocator);
    }
}
