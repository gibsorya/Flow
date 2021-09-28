#include "logicalDevice.hpp"
#include "../../root.hpp"

// namespace flow::vulkan::devices {
//     void createLogicalDevice(){
//         QueueFamilyIndicies indices = findQueueFamilies(root->flowPDs->physicalDevice);

//         root->flowDevice->queueCreateInfo = vk::DeviceQueueCreateInfo({}, indices.graphicsFamily.value(), 1, &indices.queuePriority);
//         root->flowDevice->deviceInfo = vk::DeviceCreateInfo({}, 1, &root->flowDevice->queueCreateInfo, 0, nullptr, 0, nullptr, &root->flowPDs->deviceFeatures.features);

//         if(root->validLayers->enabledValidationLayers){
//             root->flowDevice->deviceInfo.enabledLayerCount = static_cast<u32>(root->validLayers->layers.size());
//             root->flowDevice->deviceInfo.ppEnabledLayerNames = root->validLayers->layers.data();
//         }

//         if(root->flowPDs->physicalDevice.createDevice(&root->flowDevice->deviceInfo, nullptr, &root->flowDevice->device) != vk::Result::eSuccess){
//             throw std::runtime_error("Failed to create logical device!");
//         }

//         root->flowDevice->device.getQueue(indices.graphicsFamily.value(), 0, &root->flowDevice->graphicsQueue);
//     }
// }