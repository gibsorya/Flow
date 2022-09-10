#ifndef FLOW_VK_PIPELINES
#define FLOW_VK_PIPELINES

#include <foundation.hpp>
#include <rendering_device.hpp>

struct FlowVkGraphics
{
  std::vector<vk::Pipeline> graphicsPipelines;
  std::vector<vk::RenderPass> renderPasses;
  std::vector<vk::PipelineLayout> pipelineLayouts;
};

namespace flow::vulkan
{
  std::vector<char> readFile(const std::string &fileName);

  namespace pipelines
  {
    Error createGraphicsPipeline(vk::Pipeline &pipeline, vk::Device device, vk::PipelineLayout layout, vk::RenderPass renderPass, PipelinePrimitive pPrimitive, vk::Extent2D pSwapExtent, const PipelineRasterizationState &pRasterizationState,
                                 const PipelineMultisampleState &pMultisampleState, const PipelineDepthStencilState &pDepthStencilState,
                                 std::string vertexShaderFile, std::string fragShaderFile);
    Error createShaderModule(vk::ShaderModule &shaderModule, const std::vector<char> &code, vk::Device device);
    Error createPipelineLayout(vk::PipelineLayout &pipelineLayout, vk::Device device, vk::DescriptorSetLayout setLayout);
    Error createRenderPass(vk::RenderPass &renderPass, vk::Device device, vk::PhysicalDevice physicalDevice, vk::Format swapchainImageFormat);
  }
}

#endif