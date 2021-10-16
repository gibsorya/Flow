#ifndef FLOW_SYNC_OBJECTS
#define FLOW_SYNC_OBJECTS

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>

global const int MAX_FRAMES_IN_FLIGHT = 2;

struct FlowSyncObjects
{
    std::vector<vk::Semaphore> imageAvailableSemaphores;
    std::vector<vk::Semaphore> renderFinishedSemaphores;
    std::vector<vk::Fence> inFlightFences;
    std::vector<vk::Fence> imagesInFlight;

    size_t currentFrame = 0;

    bool framebufferResized = false;
};


namespace flow::vulkan {
    

    namespace syncobjects {
        Error createSyncObjects(FlowSyncObjects &syncObjects, vk::Device device, std::vector<vk::Image> swapchainImages);
    }
}

#endif