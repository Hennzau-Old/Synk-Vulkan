#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/SwapChain.h"

#include "core/rendering/CommandBuffers.h"

class Submit
{
    public:

        /* static variables */



        /* structure */

        struct SubmitCreateInfo
        {
            LogicalDevice*        logicalDevice;
            SwapChain*            swapChain;
        };

        /* functions */

        Submit();
        ~Submit();

        void                      clean();
        void                      setData(const SubmitCreateInfo& createInfo);

        void                      submit(CommandBuffers& commandBuffers);

        static int                createSubmit(Submit* submit, const SubmitCreateInfo& createInfo);

        SubmitCreateInfo&         getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createSemaphores();

        /* variables */

        VkSemaphore               m_imageAvailableSemaphore;
        VkSemaphore               m_renderFinishedSemaphore;

        SubmitCreateInfo          m_info;
};
