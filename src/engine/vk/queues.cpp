#include "queues.hpp"
#include "../root.hpp"

namespace flow::vulkan{
    QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device){
        QueueFamilyIndicies indices;

        
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
        for(auto& queueFamily : queueFamilies){
            queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        }
        vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for(const auto& queueFamily : queueFamilies){
            if(queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, root.flowSurfaces.surfaces.at(0), &presentSupport);

            if(presentSupport){
                indices.presentFamily = i;
            }

            if(indices.isComplete()){
                break;
            }

            i++;
        }

        return indices;
    }
}