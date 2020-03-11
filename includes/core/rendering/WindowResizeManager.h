#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/SwapChain.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/CommandBuffers.h"

class WindowResizeManager
{
    public:

        /* static variables */

        typedef void (*FnPtr) (CommandBuffers* commandBuffers);

        /* structure */

        struct WindowResizeManagerCreateInfo
        {
            LogicalDevice*            logicalDevice;
            SwapChain*                swapChain;
            RenderPass*               renderPass;
            Pipeline*                 pipeline;
            CommandBuffers*           commandBuffers;

            std::vector<Framebuffer>* framebuffers;

            FnPtr                     drawFunction;            
        };

        /* functions */

        WindowResizeManager();
        ~WindowResizeManager();

        void                      clean();
        void                      setData(const WindowResizeManagerCreateInfo& createInfo);

        void                      recreateComponents();

        static int                createWindowResizeManager(WindowResizeManager* windowResizeManager, const WindowResizeManagerCreateInfo& createInfo);

        WindowResizeManagerCreateInfo&         getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        /* variables */

        WindowResizeManagerCreateInfo          m_info;
};
