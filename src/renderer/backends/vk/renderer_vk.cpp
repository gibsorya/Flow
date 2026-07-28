#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

#include <iostream>
#include <cstdio>

#include "renderer/renderer.h"
#include "platform/window.h"

#include "renderer/backends/vk/helpers.h"
#include "renderer/backends/vk/engine/instance.h"

namespace
{
    struct VKState {
        VkInstance instance{VK_NULL_HANDLE};
        VkDebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
        VkPhysicalDevice physicalGPU{VK_NULL_HANDLE};
        VkSurfaceKHR surface;
    };
    VKState vk_state;

    bool vk_init(const NativeWindowInfo* native)
    {
        VkResult result;
        
        engine::createVulkanInstance(native, vk_state.instance);

        VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = engine::createDebugMessengerInfo();

        checkVkResult(engine::CreateDebugUtilsMessengerEXT(vk_state.instance, &debugMessengerInfo, nullptr, &vk_state.debugMessenger));

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
        engine::DestroyDebugUtilsMessengerEXT(vk_state.instance, vk_state.debugMessenger, nullptr);
        vkDestroySurfaceKHR(vk_state.instance, vk_state.surface, nullptr);
        vkDestroyInstance(vk_state.instance, nullptr);
    }

    void render_frame(const FramePacket* packet)
    {
        
    }

    
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