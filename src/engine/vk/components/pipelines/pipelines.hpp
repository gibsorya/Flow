#ifndef FLOW_PIPELINES
#define FLOW_PIPELINES

#include <engine/vk/rendering_device.hpp>
#include <flow/foundation.hpp>
#include <vulkan/vulkan.hpp>

struct FlowGraphics
{
    std::vector<vk::Pipeline> graphicsPipelines;
    std::vector<vk::RenderPass> renderPasses;
    std::vector<vk::PipelineLayout> pipelineLayouts;

    struct FlowGraphicsInfos
    {
        std::vector<std::vector<vk::PipelineShaderStageCreateInfo>> shaderStages;
        std::vector<vk::PipelineVertexInputStateCreateInfo> vertexInputs;
        std::vector<vk::PipelineInputAssemblyStateCreateInfo> inputAssemblies;
        std::vector<vk::PipelineTessellationStateCreateInfo> tessallationStates;
        std::vector<vk::PipelineViewportStateCreateInfo> viewportStates;
        std::vector<vk::PipelineRasterizationStateCreateInfo> rasterizations;
        std::vector<vk::PipelineMultisampleStateCreateInfo> multisamplers;
        std::vector<vk::PipelineDepthStencilStateCreateInfo> depthStencils;
        std::vector<vk::PipelineColorBlendStateCreateInfo> colorBlends;
        std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments;
        std::vector<vk::PipelineDynamicStateCreateInfo> dynamicStates;

        std::vector<vk::PipelineShaderStageCreateInfo> vertexShaderInfos;
        std::vector<vk::PipelineShaderStageCreateInfo> fragShaderInfos;

        std::vector<vk::PipelineLayout> layouts;
        std::vector<u32> subpasses;
    };

    FlowGraphicsInfos infos;
};

struct FlowComputes
{
    std::vector<VkPipeline> computePipelines;
};

struct FlowRayTracing
{
    std::vector<VkPipeline> rayTracingPipelines;
};

namespace flow::vulkan
{
    std::vector<char> readFile(const std::string &fileName);

    extern const vk::SampleCountFlagBits sampleCount[SAMPLES_MAX];
    extern const vk::CompareOp compareOperators[COMPARE_OP_MAX];
    extern const vk::StencilOp stencilOperators[STENCIL_OP_MAX];

    namespace pipelines
    {
        Error createShaderModule(vk::ShaderModule &shaderModule, const std::vector<char> &code, vk::Device device);

        Error createGraphicsPipeline(vk::Pipeline &pipeline, vk::Device device, vk::PipelineLayout layout, vk::RenderPass renderPass, PipelinePrimitive pPrimitive, vk::Extent2D pSwapExtent, const PipelineRasterizationState &pRasterizationState, const PipelineMultisampleState &pMultisampleState, const PipelineDepthStencilState &pDepthStencilState);

        Error createPipelineLayout(vk::PipelineLayout &pipelineLayout, vk::Device device, vk::DescriptorSetLayout setLayout);

        Error createRenderPass(vk::RenderPass &renderPass, vk::Device device, vk::Format swapchainImageFormat);

        Error createViewport(vk::Viewport &viewport, vk::Extent2D extent);
        
        Error createScissor(vk::Rect2D &scissor, vk::Extent2D extent);
    }
}

// struct FlowShaderModules
// {
//     std::vector<VkShaderModule> vertexShaderModules;
//     std::vector<VkShaderModule> fragShaderModules;
// };

// struct FlowScissors
// {
//     std::vector<VkRect2D> scissors; //Scissors needed for viewport states;
// };

// struct FlowViewports
// {
//     std::vector<VkViewport> viewports; //Viewports also needed for viewport states.
// };

// namespace flow::vulkan::pipelines
// {
//     //This method is for specifically reading shader files.
//     local std::vector<char> readFile(const std::string &fileName);

//     VkShaderModule createShaderModule(const std::vector<char> &code);

//     VkPipeline createGraphicsPipeline();

//     VkViewport createViewport();

//     VkRect2D createScissor();

//     VkPipelineLayout createPipelineLayout();
// }

#endif // FLOW_PIPELINES