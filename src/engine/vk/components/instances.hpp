#ifndef FLOW_INSTANCES
#define FLOW_INSTANCES

#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

/* 
Stores our vulkan instance(s) and its related components :)
*/
struct FlowInstances{
    /*
    * instances.at(0) = main Flow instance

    ! Stored in vector in case we do need to support multiple Vulkan instances
    */
    std::vector<vk::Instance> instances;

    std::vector<vk::DebugUtilsMessengerEXT> debugMessengers; //Every instance should have its own debug messenger
    std::vector<const char *> extensions; //*This could be stored inside another vector, but for now all extensions will be the same for each potential instance
};

namespace flow::vulkan::instances {
    Error createInstance(std::vector<vk::Instance> &instances, const char *appName, const char *engineName, std::vector<const char *> extensions);
    Error createInstance(std::vector<vk::Instance>& instances, const char * appName, const char * engineName, vk::InstanceCreateInfo *instanceInfo, std::vector<const char *> extensions);

    // void populateInstanceCreateInfo(vk::InstanceCreateInfo& createInfo);

    Error findInstanceExtensions(std::vector<const char *> &extensions);

    Error setupDebugMessenger(vk::Instance instance, std::vector<vk::DebugUtilsMessengerEXT> &debugMessengers);

    void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo);

    VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks* pAllocator);

    
}

#endif