#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

class CommandPool
{
    public:

        /* static variables */



        /* structure */

        struct CommandPoolCreateInfo
        {
            PhysicalDevice*       physicalDevice;
            LogicalDevice*        logicalDevice;
        };

        /* functions */

        CommandPool();
        ~CommandPool();

        void                      clean();
        void                      setData(const CommandPoolCreateInfo& createInfo);

        static int                createCommandPool(CommandPool* commandPool, const CommandPoolCreateInfo& createInfo);

        inline VkCommandPool&     getCommandPool() { return m_commandPool; }

        CommandPoolCreateInfo&    getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createCommandPool();

        /* variables */

        VkCommandPool             m_commandPool;

        CommandPoolCreateInfo     m_info;
};
