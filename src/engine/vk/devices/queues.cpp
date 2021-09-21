#include "queues.hpp"
#include "../../root.hpp"

namespace flow::vulkan::devices{
    QueueFamilyIndicies findQueueFamilies(vk::PhysicalDevice device){
        QueueFamilyIndicies indices;

        uint32_t queueFamilyCount = 0;
        device.getQueueFamilyProperties2(&queueFamilyCount, nullptr);
        std::vector<vk::QueueFamilyProperties2> queueFamilies(queueFamilyCount);
        device.getQueueFamilyProperties2(&queueFamilyCount, queueFamilies.data());

        int i = 0;
        for(const auto& queueFamily : queueFamilies){
            if(queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics){
                indices.graphicsFamily = i;
            }

            if(indices.isComplete()){
                break;
            }

            i++;
        }

        return indices;
    }
}