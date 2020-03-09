#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Window.h"
#include "core/Instance.h"
#include "core/Surface.h"
#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"
#include "core/SwapChain.h"

class CoreComponents
{
    public:

        /* static variables */



        /* structure */

        struct CoreComponentsCreateInfo
        {
            Window::WindowCreateInfo  windowCreateInfo;

            VkImageUsageFlags         imageUsage        = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        };

        /* functions */

        CoreComponents();
        ~CoreComponents();

        void                      clean();
        void                      setData(const CoreComponentsCreateInfo& createInfo);

        static int                createCoreComponents(CoreComponents* coreComponents, const CoreComponentsCreateInfo& createInfo);

        inline Window&            getWindow         () { return m_window;         }
        inline Instance&          getInstance       () { return m_instance;       }
        inline Surface&           getSurface        () { return m_surface;        }
        inline PhysicalDevice&    getPhysicalDevice () { return m_physicalDevice; }
        inline LogicalDevice&     getLogicalDevice  () { return m_logicalDevice;  }
        inline SwapChain&         getSwapChain      () { return m_swapChain;      }

        CoreComponentsCreateInfo& getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createWindow();
        int                       createInstance();
        int                       createSurface();
        int                       createPhysicalDevice();
        int                       createLogicalDevice();
        int                       createSwapChain();
        int                       endCreation();

        /* variables */

        Window                    m_window;
        Instance                  m_instance;
        Surface                   m_surface;
        PhysicalDevice            m_physicalDevice;
        LogicalDevice             m_logicalDevice;
        SwapChain                 m_swapChain;

        CoreComponentsCreateInfo  m_info;
};
