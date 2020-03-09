#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"

class RenderPass
{
    public:

        /* static variables */



        /* structure */

        struct RenderPassAttachmentsInfo
        {
            VkFormat                format;
            VkSampleCountFlagBits   samples;
            VkAttachmentLoadOp      loadOp;
            VkAttachmentStoreOp     storeOp;
            VkAttachmentLoadOp      stencilLoadOp;
            VkAttachmentStoreOp     stencilStoreOp;
            VkImageLayout           initialLayout;
            VkImageLayout           finalLayout;
        };

        struct RenderPassCreateInfo
        {
            LogicalDevice*              logicalDevice   = nullptr;

            RenderPassAttachmentsInfo   attachmentsInfo;
        };

        /* functions */

        RenderPass();
        ~RenderPass();

        void                      clean();
        void                      setData(const RenderPassCreateInfo& createInfo);

        static int                createRenderPass(RenderPass* renderPass, const RenderPassCreateInfo& createInfo);

        inline VkRenderPass&      getRenderPass() { return m_renderPass; }

        RenderPassCreateInfo&     getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createRenderPass();

        /* variables */

        VkRenderPass              m_renderPass;

        RenderPassCreateInfo      m_info;
};
