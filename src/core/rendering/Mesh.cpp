#include "core/rendering/Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::clean()
{
    m_vertexBuffer.clean();
    m_indexBuffer.clean();
}

void Mesh::setData(const MeshCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Mesh::createVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_info.vertices[0]) * m_info.vertices.size();

    Buffer stagingBuffer;

    Buffer::BufferCreateInfo stagingBufferCreateInfo = {};
    stagingBufferCreateInfo.physicalDevice           = m_info.physicalDevice;
    stagingBufferCreateInfo.logicalDevice            = m_info.logicalDevice;
    stagingBufferCreateInfo.commandPool              = m_info.commandPool;
    stagingBufferCreateInfo.size                     = bufferSize;
    stagingBufferCreateInfo.usage                    = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferCreateInfo.properties               = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (Buffer::createBuffer(&stagingBuffer, stagingBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createVertexBuffer", "createBuffer[stagingBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyData(m_info.vertices);

    Buffer::BufferCreateInfo vertexBufferCreateInfo = stagingBufferCreateInfo;
    vertexBufferCreateInfo.usage                    = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vertexBufferCreateInfo.properties               = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (Buffer::createBuffer(&m_vertexBuffer, vertexBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createVertexBuffer", "createBuffer[vertexBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyToBuffer(&m_vertexBuffer);

    stagingBuffer.clean();

    return 0;
}

int Mesh::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_info.indices[0]) * m_info.indices.size();

    Buffer stagingBuffer;

    Buffer::BufferCreateInfo stagingBufferCreateInfo = {};
    stagingBufferCreateInfo.physicalDevice           = m_info.physicalDevice;
    stagingBufferCreateInfo.logicalDevice            = m_info.logicalDevice;
    stagingBufferCreateInfo.commandPool              = m_info.commandPool;
    stagingBufferCreateInfo.size                     = bufferSize;
    stagingBufferCreateInfo.usage                    = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferCreateInfo.properties               = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (Buffer::createBuffer(&stagingBuffer, stagingBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createIndexBuffer", "createBuffer[stagingBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyData(m_info.indices);

    Buffer::BufferCreateInfo indexBufferCreateInfo  = stagingBufferCreateInfo;
    indexBufferCreateInfo.usage                     = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    indexBufferCreateInfo.properties                = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (Buffer::createBuffer(&m_indexBuffer, indexBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createIndexBuffer", "createBuffer[indexBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyToBuffer(&m_indexBuffer);

    stagingBuffer.clean();

    return 0;
}

int Mesh::createMesh(Mesh* mesh, const MeshCreateInfo& createInfo)
{
    mesh->setData(createInfo);

    return  mesh->createVertexBuffer() +
            mesh->createIndexBuffer();
}

Mesh::MeshCreateInfo& Mesh::getInfo()
{
  return m_info;
}
