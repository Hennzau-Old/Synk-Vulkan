#include "core/rendering/buffers/Buffer.h"

Buffer::Buffer()
{

}

Buffer::~Buffer()
{

}

void Buffer::clean()
{
    vkDestroyBuffer(m_info.logicalDevice->getLogicalDevice(), m_buffer, nullptr);

    Logger::printInfo("Buffer::clean", "vkDestroyBuffer!");

    vkFreeMemory(m_info.logicalDevice->getLogicalDevice(), m_bufferMemory, nullptr);

    Logger::printInfo("Buffer::clean", "vkFreeMemory!");
}

void Buffer::setData(const BufferCreateInfo& createInfo)
{
    m_info = createInfo;
}

void Buffer::copyToBuffer(Buffer* dstBuffer)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool                 = m_info.commandPool->getCommandPool();
    allocInfo.commandBufferCount          = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_info.logicalDevice->getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo  = {};
    beginInfo.sType                     = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                     = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset    = 0;
    copyRegion.dstOffset    = 0;
    copyRegion.size         = m_info.size;

    vkCmdCopyBuffer(commandBuffer, m_buffer, dstBuffer->getBuffer(), 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo       = {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit(m_info.logicalDevice->getTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_info.logicalDevice->getTransferQueue());

    vkFreeCommandBuffers(m_info.logicalDevice->getLogicalDevice(), m_info.commandPool->getCommandPool(), 1, &commandBuffer);
}

int Buffer::createBuffer()
{
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size               = m_info.size;
    bufferCreateInfo.usage              = m_info.usage;
    bufferCreateInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_info.logicalDevice->getLogicalDevice(), &bufferCreateInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        Logger::printError("Buffer::createBuffer", "vkCreateBuffer failed!");

        return 1;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_info.logicalDevice->getLogicalDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo  = {};
    allocInfo.sType                 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize        = memRequirements.size;
    allocInfo.memoryTypeIndex       = findMemoryType(memRequirements.memoryTypeBits, m_info.properties);

    if (vkAllocateMemory(m_info.logicalDevice->getLogicalDevice(), &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS)
    {
        Logger::printError("Buffer::createBuffer", "vkAllocateMemory failed!");

        return 1;
    }

    vkBindBufferMemory(m_info.logicalDevice->getLogicalDevice(), m_buffer, m_bufferMemory, 0);

    return 0;
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_info.physicalDevice->getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties))
        {
            return i;
        }
    }

    Logger::printError("Buffer::findMemoryType", "No memory type can be used for the buffer!");

    return 0;
}

int Buffer::createBuffer(Buffer* buffer, const BufferCreateInfo& createInfo)
{
    buffer->setData(createInfo);

    return buffer->createBuffer();
}

Buffer::BufferCreateInfo& Buffer::getInfo()
{
  return m_info;
}
