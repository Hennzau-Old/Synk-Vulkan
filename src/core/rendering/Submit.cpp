#include "core/rendering/Submit.h"

Submit::Submit()
{

}

Submit::~Submit()
{

}

void Submit::clean()
{
    vkDestroySemaphore(m_info.logicalDevice->getLogicalDevice(), m_renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(m_info.logicalDevice->getLogicalDevice(), m_imageAvailableSemaphore, nullptr);
}

void Submit::setData(const SubmitCreateInfo& createInfo)
{
    m_info = createInfo;
}

void Submit::submit(CommandBuffers& commandBuffers)
{
    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_info.logicalDevice->getLogicalDevice(), m_info.swapChain->getSwapChain(), UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSemaphore waitSemaphores[] =
    {
        m_imageAvailableSemaphore
    };

    VkSemaphore signalSemaphores[] =
    {
        m_renderFinishedSemaphore
    };

    VkPipelineStageFlags waitStages[] =
    {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo submitInfo           = {};
    submitInfo.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount     = 1;
    submitInfo.pWaitSemaphores        = waitSemaphores;
    submitInfo.pWaitDstStageMask      = waitStages;
    submitInfo.commandBufferCount     = 1;
    submitInfo.pCommandBuffers        = &commandBuffers.getCommandBuffers()[imageIndex];
    submitInfo.signalSemaphoreCount   = 1;
    submitInfo.pSignalSemaphores      = signalSemaphores;

    if (vkQueueSubmit(m_info.logicalDevice->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        Logger::printError("Submit::Submit", "vkQueueSubmit failed!");
    }

    VkSwapchainKHR swapChains[] =
    {
        m_info.swapChain->getSwapChain()
    };

    VkPresentInfoKHR presentInfo    = {};
    presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount  = 1;
    presentInfo.pWaitSemaphores     = signalSemaphores;
    presentInfo.swapchainCount      = 1;
    presentInfo.pSwapchains         = swapChains;
    presentInfo.pImageIndices       = &imageIndex;
    presentInfo.pResults            = nullptr;

    vkQueuePresentKHR(m_info.logicalDevice->getPresentQueue(), &presentInfo);
}

int Submit::createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(m_info.logicalDevice->getLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS)
    {
        Logger::printError("Submit::createSemaphores", "vkCreateSemaphore[imageAvailable] failed!");

        return 1;
    }

    if (vkCreateSemaphore(m_info.logicalDevice->getLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS)
    {
        Logger::printError("Submit::createSemaphores", "vkCreateSemaphore[renderFinished] failed!");

        return 1;
    }

    return 0;
}

int Submit::createSubmit(Submit* submit, const SubmitCreateInfo& createInfo)
{
    submit->setData(createInfo);

    return submit->createSemaphores();
}

Submit::SubmitCreateInfo& Submit::getInfo()
{
  return m_info;
}
