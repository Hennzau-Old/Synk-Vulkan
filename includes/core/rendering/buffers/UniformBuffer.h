#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

#include "core/rendering/CommandPool.h"
#include "core/rendering/buffers/Buffer.h"

template <class T>
class UniformBuffer
{
    public:

        /* static variables */



        /* structure */

        struct UniformBufferCreateInfo
        {
            PhysicalDevice*         physicalDevice  = nullptr;
            LogicalDevice*          logicalDevice   = nullptr;
            CommandPool*            commandPool     = nullptr;

            std::vector<T>        objects;
        };

        /* functions */

        UniformBuffer();
        ~UniformBuffer();

        void                      clean();
        void                      setData(const UniformBufferCreateInfo& createInfo);

        void                      update(const uint32_t& index, const T& object);

        static int                createUniformBuffer(UniformBuffer* uniformBuffer, const UniformBufferCreateInfo& createInfo);

        inline Buffer&            getBuffer() { return m_buffer; }

        UniformBufferCreateInfo&  getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createUniformBuffer();

        /* variables */

        Buffer                    m_buffer;

        UniformBufferCreateInfo   m_info;
};

template<class T>
UniformBuffer<T>::UniformBuffer()
{

}

template<class T>
UniformBuffer<T>::~UniformBuffer()
{

}

template<class T>
void UniformBuffer<T>::clean()
{
    m_buffer.clean();
}

template<class T>
void UniformBuffer<T>::setData(const UniformBufferCreateInfo& createInfo)
{
    m_info = createInfo;
}

template<class T>
void UniformBuffer<T>::update(const uint32_t& index, const T& object)
{
    m_buffer.copyData(object.values());
}

template<class T>
int UniformBuffer<T>::createUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_info.objects[0]) * m_info.objects.size();

    Buffer::BufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.physicalDevice           = m_info.physicalDevice;
    bufferCreateInfo.physicalDevice           = m_info.physicalDevice;
    bufferCreateInfo.logicalDevice            = m_info.logicalDevice;
    bufferCreateInfo.commandPool              = m_info.commandPool;
    bufferCreateInfo.size                     = bufferSize;
    bufferCreateInfo.usage                    = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferCreateInfo.properties               = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (Buffer::createBuffer(&m_buffer, bufferCreateInfo) != 0)
    {
        Logger::printError("UniformBuffer::createUniformBuffer", "createBuffer failed!");

        return 1;
    }

    std::vector<float> datas;

    for (unsigned int i = 0; i < m_info.objects.size(); i++)
    {
        for (float j : m_info.objects[i].values())
        {
            datas.push_back(j);
        }
    }

    m_buffer.copyData(datas);

    return 0;
}

template<class T>
int UniformBuffer<T>::createUniformBuffer(UniformBuffer* uniformBuffer, const UniformBufferCreateInfo& createInfo)
{
    uniformBuffer->setData(createInfo);

    return uniformBuffer->createUniformBuffer();
}

template<class T>
typename UniformBuffer<T>::UniformBufferCreateInfo& UniformBuffer<T>::getInfo()
{
    return m_info;
}
