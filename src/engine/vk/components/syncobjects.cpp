#include "syncobjects.hpp"

namespace flow::vulkan::syncobjects {
        Error createSyncObjects(FlowSyncObjects &syncObjects, vk::Device device, std::vector<vk::Image> swapchainImages){
            syncObjects.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
            syncObjects.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
            syncObjects.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
            syncObjects.imagesInFlight.resize(swapchainImages.size(), VK_NULL_HANDLE);

            vk::SemaphoreCreateInfo semaphoreInfo;

            vk::FenceCreateInfo fenceInfo;
            fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
                if(device.createSemaphore(&semaphoreInfo, nullptr, &syncObjects.imageAvailableSemaphores[i]) != vk::Result::eSuccess || device.createSemaphore(&semaphoreInfo, nullptr, &syncObjects.renderFinishedSemaphores[i]) != vk::Result::eSuccess || device.createFence(&fenceInfo, nullptr, &syncObjects.inFlightFences[i]) != vk::Result::eSuccess){
                    return ERR_CANT_CREATE;
                }
            }

            return SUCCESS;
        }
}
