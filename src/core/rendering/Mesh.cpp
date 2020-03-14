#include "core/rendering/Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::clean()
{
    vkDestroyBuffer(m_info.logicalDevice->getLogicalDevice(), m_vertexBuffer, nullptr);

    Logger::printInfo("Mesh::clean", "vkDestroyBuffer!");

    vkFreeMemory(m_info.logicalDevice->getLogicalDevice(), m_vertexBufferMemory, nullptr);

    Logger::printInfo("Mesh::clean", "vkFreeMemory!");

    vkDestroyBuffer(m_info.logicalDevice->getLogicalDevice(), m_indexBuffer, nullptr);

    Logger::printInfo("Mesh::clean", "vkDestroyBuffer!");

    vkFreeMemory(m_info.logicalDevice->getLogicalDevice(), m_indexBufferMemory, nullptr);

    Logger::printInfo("Mesh::clean", "vkFreeMemory!");
}

void Mesh::setData(const MeshCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Mesh::createMesh()
{
    VkDeviceSize bufferSize = sizeof(m_info.vertices[0]) * m_info.vertices.size();

    VkBuffer        stagingBuffer;
    VkDeviceMemory  stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_info.logicalDevice->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);

        memcpy(data, m_info.vertices.data(), (size_t) bufferSize);

    vkUnmapMemory(m_info.logicalDevice->getLogicalDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

    copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    vkDestroyBuffer(m_info.logicalDevice->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(m_info.logicalDevice->getLogicalDevice(), stagingBufferMemory, nullptr);

    return 0;
}

int Mesh::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_info.indices[0]) * m_info.indices.size();

    VkBuffer        stagingBuffer;
    VkDeviceMemory  stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_info.logicalDevice->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);

        memcpy(data, m_info.indices.data(), (size_t) bufferSize);

    vkUnmapMemory(m_info.logicalDevice->getLogicalDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

    copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    vkDestroyBuffer(m_info.logicalDevice->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(m_info.logicalDevice->getLogicalDevice(), stagingBufferMemory, nullptr);

    return 0;
}

void Mesh::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size               = size;
    bufferCreateInfo.usage              = usage;
    bufferCreateInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_info.logicalDevice->getLogicalDevice(), &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        Logger::printError("Mesh::createBuffer", "vkCreateBuffer failed!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_info.logicalDevice->getLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo  = {};
    allocInfo.sType                 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize        = memRequirements.size;
    allocInfo.memoryTypeIndex       = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_info.logicalDevice->getLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        Logger::printError("Mesh::createBuffer", "vkAllocateMemory failed!");
    }

    vkBindBufferMemory(m_info.logicalDevice->getLogicalDevice(), buffer, bufferMemory, 0);
}

void Mesh::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
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
    copyRegion.size         = size;

    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo       = {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit(m_info.logicalDevice->getTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_info.logicalDevice->getTransferQueue());

    vkFreeCommandBuffers(m_info.logicalDevice->getLogicalDevice(), m_info.commandPool->getCommandPool(), 1, &commandBuffer);
}

uint32_t Mesh::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

    Logger::printError("Mesh::findMemoryType", "No memory type can be used for the buffer!");

    return 0;
}

int Mesh::createMesh(Mesh* mesh, const MeshCreateInfo& createInfo)
{
    mesh->setData(createInfo);

    return  mesh->createMesh() +
            mesh->createIndexBuffer();
}

Mesh::MeshCreateInfo& Mesh::getInfo()
{
  return m_info;
}
