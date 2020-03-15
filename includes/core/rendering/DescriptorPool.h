#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

class DescriptorPool
{
    public:

        /* static variables */



        /* structure */

        struct DescriptorPoolCreateInfo
        {

        };

        /* functions */

        DescriptorPool();
        ~DescriptorPool();

        void                      clean();
        void                      setData(const DescriptorPoolCreateInfo& createInfo);

        static int                createDescriptorPool(DescriptorPool* descriptorPool, const DescriptorPoolCreateInfo& createInfo);

        inline VkDescriptorPool&  getDescriptorPool() { return m_descriptorPool; }

        DescriptorPoolCreateInfo& getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createDescriptorPool();

        /* variables */

        VkDescriptorPool          m_descriptorPool;

        DescriptorPoolCreateInfo  m_info;
};
