#pragma once

#include <vulkan/vulkan.h>
#include <cstdio>
#include <cstdlib>

inline void checkVkResult(const VkResult result, const char* what = "Vulkan call")
{
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "%s failed: VkResult %d\n", what, result);
        exit(result);
    }
}

inline bool isSupportedLayer(const char* requestedLayerName)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    for (const VkLayerProperties& layer: layers) {
        const char *layerName = layer.layerName;
        if (strcmp(layerName, requestedLayerName) == 0) {
            return true;
            break;
        }
    }

    std::cout << "Requested layer is not supported (may need to install vulkan sdk): " << requestedLayerName << "\n";
    return false;
}