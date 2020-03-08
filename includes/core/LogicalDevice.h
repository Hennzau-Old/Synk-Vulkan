#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Instance.h"
#include "core/PhysicalDevice.h"

class LogicalDevice
{
    public:

        /* static variables */



        /* structure */

        struct LogicalDeviceCreateInfo
        {
            PhysicalDevice*       physicalDevice  = nullptr;
            Instance*             instance        = nullptr;
        };

        /* functions */

        LogicalDevice();
        ~LogicalDevice();

        void                      clean();
        void                      setData(const LogicalDeviceCreateInfo& createInfo);

        static int                createLogicalDevice(LogicalDevice* logicalDevice, const LogicalDeviceCreateInfo& createInfo);

        inline VkDevice&          getLogicalDevice() { return m_logicalDevice; }
        inline VkQueue&           getGraphicsQueue() { return m_graphicsQueue; }
        inline VkQueue&           getPresentQueue () { return m_presentQueue; }

        LogicalDeviceCreateInfo&  getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createLogicalDevice();

        /* variables */

        VkDevice                  m_logicalDevice;

        VkQueue                   m_graphicsQueue;
        VkQueue                   m_presentQueue;

        LogicalDeviceCreateInfo   m_info;
};
