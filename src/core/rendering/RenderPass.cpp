#include "core/rendering/RenderPass.h"

RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::clean()
{
    vkDestroyRenderPass(m_info.logicalDevice->getLogicalDevice(), m_renderPass, nullptr);
}

void RenderPass::setData(const RenderPassCreateInfo& createInfo)
{
    m_info = createInfo;
}

int RenderPass::createRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format                  = m_info.attachmentsInfo.format;
    colorAttachment.samples                 = m_info.attachmentsInfo.samples;
    colorAttachment.loadOp                  = m_info.attachmentsInfo.loadOp;
    colorAttachment.storeOp                 = m_info.attachmentsInfo.storeOp;
    colorAttachment.stencilLoadOp           = m_info.attachmentsInfo.stencilLoadOp;
    colorAttachment.stencilStoreOp          = m_info.attachmentsInfo.stencilStoreOp;
    colorAttachment.initialLayout           = m_info.attachmentsInfo.initialLayout;
    colorAttachment.finalLayout             = m_info.attachmentsInfo.finalLayout;

    VkAttachmentReference colorAttachmentReference  = {};
    colorAttachmentReference.attachment             = 0;
    colorAttachmentReference.layout                 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass  = {};
    subpass.pipelineBindPoint     = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount  = 1;
    subpass.pColorAttachments     = &colorAttachmentReference;

    VkSubpassDependency dependency  = {};
    dependency.srcSubpass           = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass           = 0;
    dependency.srcStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask        = 0;
    dependency.dstStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask        = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount        = 1;
    renderPassCreateInfo.pAttachments           = &colorAttachment;
    renderPassCreateInfo.subpassCount           = 1;
    renderPassCreateInfo.pSubpasses             = &subpass;
    renderPassCreateInfo.dependencyCount        = 1;
    renderPassCreateInfo.pDependencies          = &dependency;

    if (vkCreateRenderPass(m_info.logicalDevice->getLogicalDevice(), &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        Logger::printError("RenderPass::createRenderPass", "vkCreateRenderpass failed!");

        return 1;
    }

    return 0;
}

int RenderPass::createRenderPass(RenderPass* renderPass, const RenderPassCreateInfo& createInfo)
{
    renderPass->setData(createInfo);

    return renderPass->createRenderPass();
}

RenderPass::RenderPassCreateInfo& RenderPass::getInfo()
{
  return m_info;
}
