#include "core/rendering/CommandBuffers.h"

CommandBuffers::CommandBuffers()
{

}

CommandBuffers::~CommandBuffers()
{

}

void CommandBuffers::clean()
{
    vkFreeCommandBuffers(m_info.logicalDevice->getLogicalDevice(), m_info.commandPool->getCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

    Logger::printInfo("CommandBuffers::clean", "vkFreeCommandBuffers!");
}

void CommandBuffers::setData(const CommandBuffersCreateInfo& createInfo)
{
    m_info = createInfo;
}

void CommandBuffers::beginCommandBuffers(const uint32_t& index)
{
    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags                    = 0;
    commandBufferBeginInfo.pInheritanceInfo         = nullptr;

    if (vkBeginCommandBuffer(m_commandBuffers[index], &commandBufferBeginInfo) != VK_SUCCESS)
    {
        Logger::printError("CommandBuffers::beginCommandBuffers", "vkBeginCommandBuffer[" + std::to_string(index) + "] failed!");
    }
}

void CommandBuffers::beginRenderPass(const uint32_t& index, RenderPass& renderPass)
{
    VkClearValue clearColor               = { 0.0f, 0.0f, 0.0f, 1.0f };

    VkRenderPassBeginInfo renderPassInfo  = {};
    renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass             = renderPass.getRenderPass();
    renderPassInfo.framebuffer            = m_info.framebuffers->at(index).getFramebuffer();
    renderPassInfo.renderArea.offset      = {0, 0};
    renderPassInfo.renderArea.extent      = m_info.swapChain->getExtent();

    renderPassInfo.clearValueCount        = 1;
    renderPassInfo.pClearValues           = &clearColor;

    vkCmdBeginRenderPass(m_commandBuffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

int CommandBuffers::createCommandBuffers()
{
    m_commandBuffers.resize(m_info.framebuffers->size());

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

void CommandBuffers::bindPipeline(const uint32_t& index, Pipeline& pipeline)
{
    vkCmdBindPipeline(m_commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());
}

void CommandBuffers::draw(const uint32_t& index)
{
    vkCmdDraw(m_commandBuffers[index], 3, 1, 0, 0);
}

void CommandBuffers::endRenderPass(const uint32_t& index)
{
    vkCmdEndRenderPass(m_commandBuffers[index]);
}

void CommandBuffers::endCommandBuffers(const uint32_t& index)
{
    if (vkEndCommandBuffer(m_commandBuffers[index]) != VK_SUCCESS)
    {
        Logger::printError("CommandBuffers::vkEndCommandBuffer", "vkEndCommandBuffer[" + std::to_string(index) + "] failed!");
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
