#pragma once
#include "flow/flow.hpp"
#include "window.hpp"
#include "vk/instances.hpp"
#include "vk/devices.hpp"
#include "vk/queues.hpp"
#include "vk/surfaces.hpp"
#include "vk/swapchains.hpp"
#include "vk/imageviews.hpp"
#include "vk/renderpasses.hpp"
#include "vk/buffers.hpp"
#include "vk/syncobjects.hpp"
#include "vk/pipelines/pipelines.hpp"
#include "vk/pipelines/pipelineinfos.hpp"

/*

Where all the data for the components in this Vulkan implementation come together. Hence "root"

*/
struct Root
{
    FlowWindow flowWindow;
    FlowInstances flowInstances;
    DebugUtils debugUtils;
    FlowSurfaces flowSurfaces;
    FlowDevices flowDevices;
    FlowSwapchains flowSwaps;
    FlowImageViews imageViews;
    FlowGraphics graphicsPipelines;
    FlowShaderModules shaderModules;
    FlowViewports flowViewports;
    FlowScissors flowScissors;
    FlowRenderPasses flowRenderPasses;
    FlowFrameBuffers flowFrameBuffers;
    FlowCommandPools flowCommandPools;
    FlowCommandBuffers flowCommandBuffers;
    FlowSyncObjects flowSyncObjects;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};
extern struct Root root;