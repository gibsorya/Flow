#include "queues.hpp"

namespace flow::vulkan::devices
{

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        u32 queueFamilyCount = 0;
        device.getQueueFamilyProperties2(&queueFamilyCount, nullptr);

        std::vector<vk::QueueFamilyProperties2> queueFamilies(queueFamilyCount);

        device.getQueueFamilyProperties2(&queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            device.getSurfaceSupportKHR(i, surface, &presentSupport);

            if(presentSupport){
                indices.presentFamily = i;
            }

            if (indices.isComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }
    //     QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device){
    //         QueueFamilyIndicies indices;

    //         u32 queueFamilyCount = 0;
    //         vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, nullptr);

    //         std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
    //         for(auto& queueFamily : queueFamilies){
    //             queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
    //         }
    //         vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, queueFamilies.data());

    //         int i = 0;
    //         for(const auto& queueFamily : queueFamilies){
    //             if(queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
    //                 indices.graphicsFamily = i;
    //             }

    //             VkBool32 presentSupport = false;
    //             vkGetPhysicalDeviceSurfaceSupportKHR(device, i, root.flowSurfaces.surfaces.at(0), &presentSupport);

    //             if(presentSupport){
    //                 indices.presentFamily = i;
    //             }

    //             if(indices.isComplete()){
    //                 break;
    //             }

    //             i++;
    //         }

    //         return indices;
    //     }
}
