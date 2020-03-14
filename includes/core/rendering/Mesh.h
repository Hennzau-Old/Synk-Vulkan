#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

#include "core/rendering/CommandPool.h"
#include "core/rendering/buffers/Buffer.h"

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

        inline Buffer&            getVertexBuffer () { return m_vertexBuffer; }
        inline Buffer&            getIndexBuffer  () { return m_indexBuffer;  }

        MeshCreateInfo&           getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createVertexBuffer();
        int                       createIndexBuffer();

        /* variables */

        Buffer                    m_vertexBuffer;
        Buffer                    m_indexBuffer;

        MeshCreateInfo            m_info;
};
