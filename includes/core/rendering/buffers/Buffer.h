#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

#include "core/rendering/CommandPool.h"

class Buffer
{
    public:

        /* static variables */



        /* structure */

        struct BufferCreateInfo
        {
            PhysicalDevice*       physicalDevice  = nullptr;
            LogicalDevice*        logicalDevice   = nullptr;
            CommandPool*          commandPool     = nullptr;

            VkDeviceSize          size;
            VkBufferUsageFlags    usage;
            VkMemoryPropertyFlags properties;
        };

        /* functions */

        Buffer();
        ~Buffer();

        void                      clean();
        void                      setData(const BufferCreateInfo& createInfo);

        template<typename T>
        void                      copyData(std::vector<T> data)
        {
            void* memData;

            vkMapMemory(m_info.logicalDevice->getLogicalDevice(), m_bufferMemory, 0, m_info.size, 0, &memData);

                memcpy(memData, data.data(), (size_t) m_info.size);

            vkUnmapMemory(m_info.logicalDevice->getLogicalDevice(), m_bufferMemory);
        }

        void                      copyToBuffer(Buffer* dstBuffer);

        static int                createBuffer(Buffer* buffer, const BufferCreateInfo& createInfo);

        inline VkBuffer&          getBuffer       () { return m_buffer;       }
        inline VkDeviceMemory&    getBufferMemory () { return m_bufferMemory; }

        BufferCreateInfo&         getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createBuffer();
        uint32_t                  findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        /* variables */

        VkBuffer                  m_buffer;
        VkDeviceMemory            m_bufferMemory;

        BufferCreateInfo          m_info;
};
