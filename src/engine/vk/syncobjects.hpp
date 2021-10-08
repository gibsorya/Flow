// #ifndef FLOW_SYNC_OBJECTS
// #define FLOW_SYNC_OBJECTS

// #include "../flow/flow.hpp"
// #include <vulkan/vulkan.hpp>


// struct FlowSyncObjects
// {
//     std::vector<VkSemaphore> imageAvailableSemaphores;
//     std::vector<VkSemaphore> renderFinishedSemaphores;
//     std::vector<VkFence> inFlightFences;
//     std::vector<VkFence> imagesInFlight;

//     size_t currentFrame = 0;

//     bool framebufferResized = false;
// };


// namespace flow::vulkan {
//     const int MAX_FRAMES_IN_FLIGHT = 2;

//     void createSyncObjects();
// }

// #endif