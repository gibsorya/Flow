#include <pipeline_infos.hpp>

namespace flow::vulkan::pipelines
{
  // Default Vertex Input Info
  void populateVertexInputInfo(vk::PipelineVertexInputStateCreateInfo &createInfo)
  {
    createInfo.vertexBindingDescriptionCount = 0;
    createInfo.pVertexBindingDescriptions = nullptr;
    createInfo.vertexAttributeDescriptionCount = 0;
    createInfo.pVertexAttributeDescriptions = nullptr;
  }
  // Default shader stage info
  void populateShaderStageInfo(vk::PipelineShaderStageCreateInfo &createInfo, vk::ShaderModule module, vk::ShaderStageFlagBits stageBits, const char *name)
  {
    createInfo.stage = stageBits;
    createInfo.module = module;
    createInfo.pName = name;
  }

  // Default Input Assembly Info
  void populateInputAssemblyInfo(vk::PipelineInputAssemblyStateCreateInfo &createInfo)
  {
    createInfo.topology = vk::PrimitiveTopology::eTriangleList;
    createInfo.primitiveRestartEnable = VK_FALSE;
  }
}