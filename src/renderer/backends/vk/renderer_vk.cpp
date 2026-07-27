#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include "renderer/renderer.h"
#include "platform/window.h"

#include <iostream>
#include <cstdio>

namespace
{
    struct VKState {
        VkInstance instance;
        VkSurfaceKHR surface;
    };
    VKState vk_state;

    bool vk_init(const NativeWindowInfo* native)
    {
        VkResult result;
        
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Unknown";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName = "Flow Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_4;

        uint32_t windowExtCount = 0;
        const char** windowExts = native->getRequiredInstanceExtensions(&windowExtCount);

        std::vector<const char*> extensions(windowExts, windowExts + windowExtCount);

        #ifdef DEBUG_BUILD
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            const bool enableValidationLayers = true;
        #else
            const bool enableValidationLayers = false;
        #endif
    
        VkInstanceCreateInfo instanceInfo;
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pNext = nullptr;
        instanceInfo.pApplicationInfo = &appInfo;
        // instanceInfo.flags = ENABLE FOR MACOS
        instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
        instanceInfo.ppEnabledExtensionNames = extensions.data();

        instanceInfo.enabledLayerCount = 0;

        result = vkCreateInstance(&instanceInfo, nullptr, &vk_state.instance);

        if(result != VK_SUCCESS)
        {
            fprintf(stderr, "Instance creation failed: %d\n", result);
            return false;
        }

        uint64_t surfaceRaw = 0;
        result = static_cast<VkResult>(
            native->createVulkanSurface((uint64_t)vk_state.instance, &surfaceRaw));

        if(result != VK_SUCCESS)
        {
            fprintf(stderr, "Surface creation failed: %d\n", result);
            return false;
        }
    
        vk_state.surface = reinterpret_cast<VkSurfaceKHR>(surfaceRaw);
        return true;
    }

    void vk_shutdown()
    {
        vkDestroySurfaceKHR(vk_state.instance, vk_state.surface, nullptr);
        vkDestroyInstance(vk_state.instance, nullptr);
    }

    void render_frame(const FramePacket* packet)
    {
        
    }

    // MeshHandle *
}

bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    return false;
}

RendererAPI createVKRenderer()
{
    return {RENDERER_API_VERSION, "Vulkan", WindowConfig{},
            vk_init, vk_shutdown};
}