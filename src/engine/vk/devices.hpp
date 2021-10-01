// #ifndef FLOW_DEVICES
// #define FLOW_DEVICES

// #include "../flow/flow.hpp"
// #include <vulkan/vulkan.h>

// typedef struct FlowDevices FlowDevices;
// struct FlowDevices {
//     VkPhysicalDeviceProperties2 deviceProperties = {
//         .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
//     };
//     VkPhysicalDeviceFeatures2 deviceFeatures = {
//         .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
//     };

//     void pickPhysicalDevice();
//     void createLogicalDevice();
    
//     [[nodiscard]] const VkPhysicalDevice& getPhysicalDevice() const;
//     [[nodiscard]] const std::vector<VkDevice>& getDevices() const;
//     [[nodiscard]] const VkQueue& getGraphicsQueue() const;
//     [[nodiscard]] const VkQueue& getPresentQueue() const;


// private:
//     VkPhysicalDevice physicalDevice;
//     std::vector<VkDevice> devices;
//     VkDevice device;
//     VkQueue graphicsQueue;
//     VkQueue presentQueue;
// };

// namespace flow::vulkan{
//     void pickPhysicalDevice();
//     bool isDeviceSuitable(VkPhysicalDevice device);

//     // VkDevice createLogicalDevice(VkPhysicalDevice *physicalDevice);
// }

// #endif