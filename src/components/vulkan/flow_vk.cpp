#include <flow_vk.hpp>

namespace flow::vulkan
{
  Error initalize(FlowVkContext *vkContext)
  {
    Error err;

    err = surfaces::initWindow(vkContext->surfaces.window, vkContext->surfaces.WIDTH, vkContext->surfaces.HEIGHT, "Flow Engine");

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

    err = devices::createLogicalDevice(vkContext->devices.devices, vkContext->devices.physicalDevices.at(0), vkContext->surfaces.surfaces.at(0), graphicsQueue, presentQueue);

    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create logical device!");

    vkContext->devices.graphicsQueues.push_back(graphicsQueue);
    vkContext->devices.presentQueues.push_back(presentQueue);

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
    vkContext->graphics.graphicsPipelines.push_back(graphicsPipeline);

    std::vector<vk::Framebuffer> frameBuffers;
    err = buffers::createFramebuffers(frameBuffers, vkContext->devices.devices.at(0), vkContext->swaps.swapchainImageViews, vkContext->swaps.swapchainExtents.at(0), vkContext->graphics.renderPasses.at(0));
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create frame buffers!");
    vkContext->frameBuffers.swapchainFrameBuffers.push_back(frameBuffers);

    return SUCCESS;
  }
}