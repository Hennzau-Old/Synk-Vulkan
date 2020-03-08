#include "core/rendering/Framebuffer.h"

Framebuffer::Framebuffer()
{

}

Framebuffer::~Framebuffer()
{

}

void Framebuffer::clean()
{
    vkDestroyFramebuffer(m_info.logicalDevice->getLogicalDevice(), m_framebuffer, nullptr);
}

void Framebuffer::setData(const FramebufferCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Framebuffer::createFramebuffer()
{
    uint32_t imageViewsSize = m_info.imageViews.size();

    VkImageView attachments[imageViewsSize];

    for (size_t i = 0; i < imageViewsSize; i++)
    {
        attachments[i] = *m_info.imageViews[i];
    }

    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass              = m_info.renderPass->getRenderPass();
    framebufferCreateInfo.attachmentCount         = imageViewsSize;
    framebufferCreateInfo.pAttachments            = attachments;
    framebufferCreateInfo.width                   = m_info.swapChain->getExtent().width;
    framebufferCreateInfo.height                  = m_info.swapChain->getExtent().height;
    framebufferCreateInfo.layers                  = 1;

    if (vkCreateFramebuffer(m_info.logicalDevice->getLogicalDevice(), &framebufferCreateInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
    {
        Logger::printError("Framebuffers::createFramebuffers", "vkCreateFramebuffer failed!");

        return 1;
    }

    return 0;
}

int Framebuffer::createFramebuffer(Framebuffer* framebuffer, const FramebufferCreateInfo& createInfo)
{
    framebuffer->setData(createInfo);

    return framebuffer->createFramebuffer();
}

Framebuffer::FramebufferCreateInfo& Framebuffer::getInfo()
{
  return m_info;
}
