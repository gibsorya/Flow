#include "syncobjects.hpp"
#include "../root.hpp"

namespace flow::vulkan
{
    void createSyncObjects()
    {
        root.flowSyncObjects.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        root.flowSyncObjects.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        root.flowSyncObjects.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        root.flowSyncObjects.imagesInFlight.resize(root.flowSwaps.swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(root.flowDevices.devices.at(0), &semaphoreInfo, nullptr, &root.flowSyncObjects.imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(root.flowDevices.devices.at(0), &semaphoreInfo, nullptr, &root.flowSyncObjects.renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(root.flowDevices.devices.at(0), &fenceInfo, nullptr, &root.flowSyncObjects.inFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create sync objects for a frame!");
            }
        }
    }

}