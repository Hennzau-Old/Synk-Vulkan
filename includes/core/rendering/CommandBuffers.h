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

#include "core/rendering/Mesh.h"

class CommandBuffers
{
    public:

        /* static variables */



        /* structure */

        struct CommandBuffersCreateInfo
        {
            std::vector<Framebuffer>*     framebuffers  = nullptr;

            LogicalDevice*                logicalDevice = nullptr;

            SwapChain*                    swapChain     = nullptr;
            CommandPool*                  commandPool   = nullptr;
        };

        /* functions */

        CommandBuffers();
        ~CommandBuffers();

        void                              clean();
        void                              setData(const CommandBuffersCreateInfo& createInfo);

        void                              beginCommandBuffers(const uint32_t& index);
        void                              beginRenderPass(const uint32_t& index, RenderPass& renderPass);

        void                              bindPipeline(const uint32_t& index, Pipeline& pipeline);
        void                              bindVertexBuffer(const uint32_t& index, std::vector<VkBuffer>& vertexBuffers);
        void                              bindIndexBuffer(const uint32_t& index, VkBuffer& indexBuffer);

        void                              draw(const uint32_t& index, const size_t& drawCount);

        void                              endCommandBuffers(const uint32_t& index);
        void                              endRenderPass(const uint32_t& index);

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
