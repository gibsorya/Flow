#include <sync_objects.hpp>

namespace flow::vulkan
{
  namespace syncobjects
  {
    Error createSyncObjects(FlowVkSyncObjects &syncObjects, vk::Device device, std::vector<vk::Image> swapchainImages)
    {
      vk::SemaphoreCreateInfo semaphoreInfo;
      vk::FenceCreateInfo fenceInfo;
      fenceInfo.flags = {vk::FenceCreateFlagBits::eSignaled};

      vk::Semaphore imageSemaphore;
      vk::Semaphore renderSemaphore;
      vk::Fence fence;

      if (device.createSemaphore(&semaphoreInfo, nullptr, &imageSemaphore) != vk::Result::eSuccess || device.createSemaphore(&semaphoreInfo, nullptr, &renderSemaphore) != vk::Result::eSuccess || device.createFence(&fenceInfo, nullptr, &fence) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }

      syncObjects.imageAvailableSemaphores.push_back(imageSemaphore);
      syncObjects.renderFinishedSemaphores.push_back(renderSemaphore);
      syncObjects.inFlightFences.push_back(fence);

      return SUCCESS;
    }
  }
}