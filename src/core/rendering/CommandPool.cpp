#include "core/rendering/CommandPool.h"

CommandPool::CommandPool()
{

}

CommandPool::~CommandPool()
{

}

void CommandPool::clean()
{
    vkDestroyCommandPool(m_info.logicalDevice->getLogicalDevice(), m_commandPool, nullptr);

    Logger::printInfo("CommandPool::clean", "vkDestroyCommandPool!");
}

void CommandPool::setData(const CommandPoolCreateInfo& createInfo)
{
    m_info = createInfo;
}

int CommandPool::createCommandPool()
{
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex        = m_info.queueFamilyIndex;
    commandPoolCreateInfo.flags                   = 0;

    if (vkCreateCommandPool(m_info.logicalDevice->getLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        Logger::printError("CommandPool::createCommandPool", "vkCreateCommandPool failed!");

        return 1;
    }

    return 0;
}

int CommandPool::createCommandPool(CommandPool* commandPool, const CommandPoolCreateInfo& createInfo)
{
    commandPool->setData(createInfo);

    return commandPool->createCommandPool();
}

CommandPool::CommandPoolCreateInfo& CommandPool::getInfo()
{
  return m_info;
}
