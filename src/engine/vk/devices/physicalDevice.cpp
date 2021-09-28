// #include "physicalDevice.hpp"
// #include "../../root.hpp"

// namespace flow::vulkan::devices {
//     void pickPhysicalDevices(){
//         u32 deviceCount;
//         root->flowInstance->instance.enumeratePhysicalDevices(&deviceCount, nullptr);

//         if(deviceCount == 0){
//             std::runtime_error("Failed to find GPU with Vulkan support!");
//         }

        
//         std::vector<vk::PhysicalDevice> devices(deviceCount);
//         root->flowInstance->instance.enumeratePhysicalDevices(&deviceCount, devices.data());

//         for(const auto& device : devices){
//             if(isDeviceSuitable(device)){
//                 root->flowPDs->physicalDevice = device;
//                 break;
//             }
//         }

//         if(&root->flowPDs->physicalDevice == VK_NULL_HANDLE){
//             throw std::runtime_error("Failed to find a suitable GPU!");
//         }
//     }

//     bool isDeviceSuitable(vk::PhysicalDevice device){
//         QueueFamilyIndicies indices = findQueueFamilies(device);

        

//         device.getProperties2(&root->flowPDs->deviceProperties);
//         device.getFeatures2(&root->flowPDs->deviceFeatures);
        
//         return root->flowPDs->deviceProperties.properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && root->flowPDs->deviceFeatures.features.geometryShader && indices.isComplete();
//     }
// }