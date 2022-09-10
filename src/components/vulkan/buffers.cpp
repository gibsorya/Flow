#include <buffers.hpp>

namespace flow::vulkan::buffers
{
  Error createFramebuffers(std::vector<vk::Framebuffer> &frameBuffers, vk::Device device, std::vector<vk::ImageView> swapchainImageViews, vk::Extent2D swapExtent, vk::RenderPass renderpass)
  {
    frameBuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
      vk::ImageView attachments[] = {
          swapchainImageViews.at(i)};

      vk::FramebufferCreateInfo createInfo;
      createInfo.renderPass = renderpass;
      createInfo.attachmentCount = 1;
      createInfo.pAttachments = attachments;
      createInfo.width = swapExtent.width;
      createInfo.height = swapExtent.height;
      createInfo.layers = 1;

      if (device.createFramebuffer(&createInfo, nullptr, &frameBuffers[i]) != vk::Result::eSuccess)
      {
        return ERR_CANT_CREATE;
      }
    }
  }
}