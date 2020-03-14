#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

#include "core/rendering/CommandPool.h"

class Mesh
{
    public:

        /* static variables */



        /* structure */

        struct MeshCreateInfo
        {
            PhysicalDevice*         physicalDevice  = nullptr;
            LogicalDevice*          logicalDevice   = nullptr;
            CommandPool*            commandPool     = nullptr;

            std::vector<float>      vertices;
            std::vector<uint16_t>   indices;
        };

        /* functions */

        Mesh();
        ~Mesh();

        void                      clean();
        void                      setData(const MeshCreateInfo& createInfo);

        static int                createMesh(Mesh* mesh, const MeshCreateInfo& createInfo);

        inline VkBuffer&          getVertexBuffer       () { return m_vertexBuffer;       }
        inline VkDeviceMemory&    getVertexBufferMemory () { return m_vertexBufferMemory; }

        inline VkBuffer&          getIndexBuffer      () { return m_indexBuffer;        }
        inline VkDeviceMemory&    getIndexBufferMemory() { return m_indexBufferMemory;  }

        MeshCreateInfo&           getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createMesh();
        int                       createIndexBuffer();

        void                      createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void                      copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        uint32_t                  findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        /* variables */

        VkBuffer                  m_vertexBuffer;
        VkDeviceMemory            m_vertexBufferMemory;

        VkBuffer                  m_indexBuffer;
        VkDeviceMemory            m_indexBufferMemory;

        MeshCreateInfo            m_info;
};
