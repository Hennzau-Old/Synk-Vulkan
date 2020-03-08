#include "core/rendering/CommandBuffers.h"

CommandBuffers::CommandBuffers()
{

}

CommandBuffers::~CommandBuffers()
{

}

void CommandBuffers::clean()
{

}

void CommandBuffers::setData(const CommandBuffersCreateInfo& createInfo)
{
    m_info = createInfo;
}

void CommandBuffers::beginCommandBuffers()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                    = 0;
        commandBufferBeginInfo.pInheritanceInfo         = nullptr;

        if (vkBeginCommandBuffer(m_commandBuffers[i], &commandBufferBeginInfo) != VK_SUCCESS)
        {
            Logger::printError("CommandBuffers::beginCommandBuffers", "vkBeginCommandBuffer[" + std::to_string(i) + "] failed!");
        }
    }
}

void CommandBuffers::beginRenderPass()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        VkClearValue clearColor               = { 0.0f, 0.0f, 0.0f, 1.0f };

        VkRenderPassBeginInfo renderPassInfo  = {};
        renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass             = m_info.renderPass->getRenderPass();
        renderPassInfo.framebuffer            = m_info.framebuffers[i].getFramebuffer();
        renderPassInfo.renderArea.offset      = {0, 0};
        renderPassInfo.renderArea.extent      = m_info.swapChain->getExtent();

        renderPassInfo.clearValueCount        = 1;
        renderPassInfo.pClearValues           = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }
}

int CommandBuffers::createCommandBuffers()
{
    m_commandBuffers.resize(m_info.framebuffers.size());

    VkCommandBufferAllocateInfo commandBufferAllocInfo  = {};
    commandBufferAllocInfo.sType                        = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool                  = m_info.commandPool->getCommandPool();
    commandBufferAllocInfo.level                        = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount           = (uint32_t) m_commandBuffers.size();

    if (vkAllocateCommandBuffers(m_info.logicalDevice->getLogicalDevice(), &commandBufferAllocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        Logger::printError("CommandBuffers::createCommandBuffers", "vkAllocateCommandBuffers failed!");

        return 1;
    }

    return 0;
}

void CommandBuffers::bindPipeline()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_info.pipeline->getPipeline());
    }
}

void CommandBuffers::draw()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);
    }
}

void CommandBuffers::endRenderPass()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        vkCmdEndRenderPass(m_commandBuffers[i]);
    }
}

void CommandBuffers::endCommandBuffers()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
        {
            Logger::printError("CommandBuffers::vkEndCommandBuffer", "vkEndCommandBuffer failed!");
        }
    }
}

int CommandBuffers::createCommandBuffers(CommandBuffers* commandBuffers, const CommandBuffersCreateInfo& createInfo)
{
    commandBuffers->setData(createInfo);

    return commandBuffers->createCommandBuffers();
}

CommandBuffers::CommandBuffersCreateInfo& CommandBuffers::getInfo()
{
  return m_info;
}
