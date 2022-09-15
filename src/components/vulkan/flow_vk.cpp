#include <flow_vk.hpp>

namespace flow::vulkan
{
  Error initalize(FlowVkContext *vkContext)
  {
    Error err;

    err = surfaces::initWindow(vkContext->surfaces.window, vkContext->surfaces.WIDTH, vkContext->surfaces.HEIGHT, "Flow Engine", &vkContext->surfaces);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window!");

    vkContext->instances.extensions = instances::findInstanceExtensions();

    err = instances::createInstance(vkContext->instances.instances, "Flow", "Flow", vkContext->instances.extensions);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create instance!");

    err = surfaces::createSurface(vkContext->surfaces.surfaces, vkContext->instances.instances.at(0), vkContext->surfaces.window);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create window surface!");

    err = instances::setupDebugMessenger(vkContext->instances.instances.at(0), vkContext->instances.debugMessengers);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to setup debug messenger!");

    err = devices::pickPhysicalDevice(vkContext->devices.physicalDevices, vkContext->instances.instances.at(0), vkContext->surfaces.surfaces.at(0));

    ERROR_FAIL_COND(err != SUCCESS, ERR_NOT_FOUND, "Failed to find physical device!");

    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    vk::Queue transferQueue;

    err = devices::createLogicalDevice(vkContext->devices.devices, vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0), graphicsQueue, presentQueue, transferQueue);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create logical device!");

    vkContext->devices.graphicsQueues.push_back(graphicsQueue);
    vkContext->devices.presentQueues.push_back(presentQueue);
    vkContext->devices.transferQueues.push_back(transferQueue);

    vk::SwapchainKHR swapchain;
    vk::Extent2D swapExtent;
    vk::Format swapFormat;

    err = swapchains::createSwapchain(swapchain, swapExtent, vkContext->swaps.swapchainImages, swapFormat, vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0), vkContext->surfaces.window);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create swapchain!");
    vkContext->swaps.swapchains.push_back(swapchain);
    vkContext->swaps.swapchainExtents.push_back(swapExtent);
    vkContext->swaps.swapchainImageFormats.push_back(swapFormat);

    err = swapchains::createImageViews(vkContext->swaps, vkContext->devices.devices.at(0));
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create image views!");

    vk::RenderPass renderPass;
    err = pipelines::createRenderPass(renderPass, vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->swaps.swapchainImageFormats.at(0));
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create render pass!");
    vkContext->graphics.renderPasses.push_back(renderPass);

    vk::PipelineLayout layout;
    err = pipelines::createPipelineLayout(layout, vkContext->devices.devices.at(0), nullptr);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create pipeline layout!");
    vkContext->graphics.pipelineLayouts.push_back(layout);

    vk::Pipeline graphicsPipeline;
    err = pipelines::createGraphicsPipeline(graphicsPipeline, vkContext->devices.devices.at(0), vkContext->graphics.pipelineLayouts.at(0), vkContext->graphics.renderPasses.at(0), PIPELINE_PRIMITIVE_TRIANGLES,
                                            vkContext->swaps.swapchainExtents.at(0), PipelineRasterizationState(), PipelineMultisampleState(), PipelineDepthStencilState(), "data/shaders/vert.spv", "data/shaders/frag.spv");
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create graphics pipeline!");
    vkContext->graphics.graphicsPipelines.push_back(graphicsPipeline);

    std::vector<vk::Framebuffer> frameBuffers;
    err = buffers::createFramebuffers(frameBuffers, vkContext->devices.devices.at(0), vkContext->swaps.swapchainImageViews, vkContext->swaps.swapchainExtents.at(0), vkContext->graphics.renderPasses.at(0));
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create frame buffers!");
    vkContext->frameBuffers.swapchainFrameBuffers.push_back(frameBuffers);

    QueueFamilyIndices indices = findQueueFamilies(vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0)); 
    std::array<u32, 2> queueFamilyIndices = {indices.graphicsFamily.value(), indices.transferFamily.value()};

    vk::CommandPool graphicsCommandPool;
    vk::CommandPool transferCommandPool;

    err = buffers::createCommandPool(graphicsCommandPool, vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0), indices.graphicsFamily.value());
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create graphics command pool!");
    vkContext->commandPools.graphicsCommandPools.push_back(graphicsCommandPool);
    err = buffers::createCommandPool(transferCommandPool, vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0), indices.transferFamily.value());
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create transfer command pool!");
    vkContext->commandPools.transferCommandPools.push_back(transferCommandPool);

    vk::Buffer vertexBuffer;
    vk::DeviceMemory vertexBufferMemory;
    err = buffers::createVertexBuffer(vertexBuffer, vertexBufferMemory, vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->commandPools.transferCommandPools.at(0), vkContext->devices.transferQueues.at(0), queueFamilyIndices);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create vertex buffer!");
    vkContext->vertexBuffers.vertexBuffers.push_back(vertexBuffer);
    vkContext->vertexBuffers.vertexMemories.push_back(vertexBufferMemory);

    vk::Buffer indexBuffer;
    vk::DeviceMemory indexBufferMemory;
    err = buffers::createIndexBuffer(indexBuffer, indexBufferMemory, vkContext->devices.devices.at(0), vkContext->devices.physicalDevices.at(0), vkContext->commandPools.transferCommandPools.at(0), vkContext->devices.transferQueues.at(0), queueFamilyIndices);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create vertex buffer!");
    vkContext->indexBuffers.indexBuffers.push_back(indexBuffer);
    vkContext->indexBuffers.indexMemories.push_back(indexBufferMemory);

    std::vector<vk::CommandBuffer> commandBuffers;
    err = buffers::createCommandBuffers(commandBuffers, vkContext->devices.devices.at(0), vkContext->commandPools.graphicsCommandPools.at(0));
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create command buffers!");
    vkContext->commandBuffers.commandBuffers.push_back(commandBuffers);

    err = syncobjects::createSyncObjects(vkContext->syncObjects, vkContext->devices.devices.at(0), vkContext->swaps.swapchainImages);
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create sync objects!");

    return SUCCESS;
  }
}