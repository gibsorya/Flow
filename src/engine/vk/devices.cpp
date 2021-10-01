// #include "devices.hpp"
// #include <engine/root.hpp>

// namespace flow::vulkan
// {
//     bool isDeviceSuitable(VkPhysicalDevice device)
//     {
//         QueueFamilyIndicies indices = findQueueFamilies(device);

//         vkGetPhysicalDeviceProperties2(device, &root->flowDevices->deviceProperties);
//         vkGetPhysicalDeviceFeatures2(device, &root->flowDevices->deviceFeatures);

//         return root->flowDevices->deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && root->flowDevices->deviceFeatures.features.geometryShader && indices.isComplete();
//     }
// }

// void FlowDevices::pickPhysicalDevice()
// {
//     u32 deviceCount;
//     vkEnumeratePhysicalDevices(root->flowInstances->getInstance(), &deviceCount, nullptr);

//     if (deviceCount == 0)
//     {
//         std::runtime_error("Failed to find GPU with Vulkan support!");
//     }

//     std::vector<VkPhysicalDevice> devices(deviceCount);
//     vkEnumeratePhysicalDevices(root->flowInstances->getInstance(), &deviceCount, devices.data());

//     for (const auto &device : devices)
//     {
//         if (flow::vulkan::isDeviceSuitable(device))
//         {
//             this->physicalDevice = device;
//             break;
//         }
//     }

//     if (this->physicalDevice == VK_NULL_HANDLE)
//     {
//         throw std::runtime_error("Failed to find a suitable GPU!");
//     }
// }

// void FlowDevices::createLogicalDevice()
// {
//     QueueFamilyIndicies indices = flow::vulkan::findQueueFamilies(this->physicalDevice);

//     std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//     std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

//     float queuePriority = 1.0f;
//     for(u32 queueFamily : uniqueQueueFamilies){
//   VkDeviceQueueCreateInfo queueCreateInfo{};
//     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//     queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
//     queueCreateInfo.queueCount = 1;
//     queueCreateInfo.pQueuePriorities = &queuePriority;
//     queueCreateInfos.push_back(queueCreateInfo);
//     }
  

//     VkDeviceCreateInfo createInfo{};
//     createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//     createInfo.pQueueCreateInfos = queueCreateInfos.data();
//     createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
//     createInfo.pEnabledFeatures = &this->deviceFeatures.features;
//     createInfo.enabledExtensionCount = 0;

//     if (flow::enabledValidationLayers)
//     {
//         createInfo.enabledLayerCount = static_cast<u32>(flow::validlayers::layers.size());
//         createInfo.ppEnabledLayerNames = flow::validlayers::layers.data();
//     }
//     else
//     {
//         createInfo.enabledLayerCount = 0;
//     }

//     if (vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->device) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to create logical device!");
//     }

//     vkGetDeviceQueue(this->device, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
//     vkGetDeviceQueue(this->device, indices.presentFamily.value(), 0, &this->presentQueue);

//     this->devices.push_back(this->device);
// }

// const std::vector<VkDevice> &FlowDevices::getDevices() const
// {
//     return devices;
// }
// const VkQueue &FlowDevices::getGraphicsQueue() const
// {
//     return graphicsQueue;
// }
// const VkQueue& FlowDevices::getPresentQueue() const
// {
//     return presentQueue;
// }
// const VkPhysicalDevice &FlowDevices::getPhysicalDevice() const
// {
//     return physicalDevice;
// }
