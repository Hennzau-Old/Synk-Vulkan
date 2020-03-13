#include "core/rendering/WindowResizeManager.h"

WindowResizeManager::WindowResizeManager()
{

}

WindowResizeManager::~WindowResizeManager()
{

}

void WindowResizeManager::clean()
{

}

void WindowResizeManager::setData(const WindowResizeManagerCreateInfo& createInfo)
{
    m_info = createInfo;
}

void WindowResizeManager::recreateComponents()
{
    m_info.logicalDevice->wait();

    for (size_t i = 0; i < m_info.framebuffers->size(); i++)
    {
        m_info.framebuffers->at(i).clean();
    }

    m_info.commandBuffers->clean();
    m_info.pipeline->clean();
    m_info.renderPass->clean();
    m_info.swapChain->clean();

    if (SwapChain::createSwapChain(m_info.swapChain, m_info.swapChain->getInfo()) != 0)
    {
        Logger::printError("WindowResizeManager::recreateComponents", "createSwapChain failed!");
    }

    m_info.renderPass->getInfo().attachmentsInfo.format = m_info.swapChain->getImageFormat();

    if (RenderPass::createRenderPass(m_info.renderPass, m_info.renderPass->getInfo()) != 0)
    {
      Logger::printError("WindowResizeManager::recreateComponents", "createRenderPass failed!");
    }

    if (Pipeline::createPipeline(m_info.pipeline, m_info.pipeline->getInfo()) != 0)
    {
      Logger::printError("WindowResizeManager::recreateComponents", "createPipeline failed!");
    }

    for (size_t i = 0; i < m_info.framebuffers->size(); i++)
    {
        if (Framebuffer::createFramebuffer(&m_info.framebuffers->at(i), m_info.framebuffers->at(i).getInfo()) != 0)
        {
            Logger::printError("main::init", "createFramebuffer failed!");
        }
    }

    m_info.commandBuffers->getInfo().framebuffers = m_info.framebuffers;

    if (CommandBuffers::createCommandBuffers(m_info.commandBuffers, m_info.commandBuffers->getInfo()) != 0)
    {
        Logger::printError("WindowResizeManager::recreateComponents", "createCommandBuffers failed!");
    }

    m_info.drawFunction(m_info.commandBuffers);

}

int WindowResizeManager::createWindowResizeManager(WindowResizeManager* windowResizeManager, const WindowResizeManagerCreateInfo& createInfo)
{
    windowResizeManager->setData(createInfo);

    return 0;
}

WindowResizeManager::WindowResizeManagerCreateInfo& WindowResizeManager::getInfo()
{
  return m_info;
}
