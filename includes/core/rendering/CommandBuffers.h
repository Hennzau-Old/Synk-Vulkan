#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/SwapChain.h"

#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/CommandPool.h"

class CommandBuffers
{
    public:

        /* static variables */



        /* structure */

        struct CommandBuffersCreateInfo
        {
            std::vector<Framebuffer>      framebuffers;

            LogicalDevice*                logicalDevice;

            SwapChain*                    swapChain;
            RenderPass*                   renderPass;
            Pipeline*                     pipeline;
            CommandPool*                  commandPool;
        };

        /* functions */

        CommandBuffers();
        ~CommandBuffers();

        void                              clean();
        void                              setData(const CommandBuffersCreateInfo& createInfo);

        void                              beginCommandBuffers();
        void                              beginRenderPass();

        void                              bindPipeline();

        void                              draw();

        void                              endCommandBuffers();
        void                              endRenderPass();

        static int                        createCommandBuffers(CommandBuffers* commandBuffers, const CommandBuffersCreateInfo& createInfo);

        CommandBuffersCreateInfo&         getInfo();

        inline std::vector<VkCommandBuffer>& getCommandBuffers() { return m_commandBuffers; }

        /* variables */

    private:

        /* structure */

        /* functions */

        int                               createCommandBuffers();

        /* variables */

        std::vector<VkCommandBuffer>      m_commandBuffers;

        CommandBuffersCreateInfo          m_info;
};
