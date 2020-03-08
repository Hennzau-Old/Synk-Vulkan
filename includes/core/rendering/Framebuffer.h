#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/SwapChain.h"
#include "core/rendering/RenderPass.h"

class Framebuffer
{
    public:

        /* static variables */



        /* structure */

        struct FramebufferCreateInfo
        {
            LogicalDevice*                  logicalDevice = nullptr;
            RenderPass*                     renderPass    = nullptr;
            SwapChain*                      swapChain     = nullptr;

            std::vector<const VkImageView*> imageViews;
        };

        /* functions */

        Framebuffer();
        ~Framebuffer();

        void                            clean();
        void                            setData(const FramebufferCreateInfo& createInfo);

        static int                      createFramebuffer(Framebuffer* framebuffer, const FramebufferCreateInfo& createInfo);

        inline VkFramebuffer&           getFramebuffer() { return m_framebuffer; }

        FramebufferCreateInfo&          getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                             createFramebuffer();

        /* variables */

        VkFramebuffer                   m_framebuffer;

        FramebufferCreateInfo           m_info;
};
