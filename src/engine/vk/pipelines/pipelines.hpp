// #ifndef FLOW_PIPELINES
// #define FLOW_PIPELINES

// #include "../../flow/flow.hpp"
// #include <vulkan/vulkan.h>

// struct FlowGraphics
// {
//     std::vector<VkPipeline> graphicsPipelines;
//     std::vector<VkPipelineLayout> pipelineLayouts;

//     struct FlowGraphicsInfos
//     {
//         std::vector<std::vector<VkPipelineShaderStageCreateInfo>> shaderStages;
//         std::vector<VkPipelineVertexInputStateCreateInfo> vertexInputs;
//         std::vector<VkPipelineInputAssemblyStateCreateInfo> inputAssemblies;
//         std::vector<VkPipelineTessellationStateCreateInfo> tessallationStates;
//         std::vector<VkPipelineViewportStateCreateInfo> viewportStates;
//         std::vector<VkPipelineRasterizationStateCreateInfo> rasterizations;
//         std::vector<VkPipelineMultisampleStateCreateInfo> multisamplers;
//         std::vector<VkPipelineDepthStencilStateCreateInfo> depthStencils;
//         std::vector<VkPipelineColorBlendStateCreateInfo> colorBlends;
//         std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
//         std::vector<VkPipelineDynamicStateCreateInfo> dynamicStates;

//         std::vector<VkPipelineShaderStageCreateInfo> vertexShaderInfos;
//         std::vector<VkPipelineShaderStageCreateInfo> fragShaderInfos;

//         std::vector<VkPipelineLayout> layouts;
//         std::vector<VkRenderPass> renderPasses;
//         std::vector<u32> subpasses;
//     };

//     FlowGraphicsInfos infos;
// };

// struct FlowComputes
// {
//     std::vector<VkPipeline> computePipelines;
// };

// struct FlowRayTracing
// {
//     std::vector<VkPipeline> rayTracingPipelines;
// };

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

// #endif // FLOW_PIPELINES