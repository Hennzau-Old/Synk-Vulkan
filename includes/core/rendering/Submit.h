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

        const int MAX_FRAMES_IN_FLIGHT  = 2;

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

        int                       createSyncObjects();

        /* variables */

        std::vector<VkSemaphore>  m_imageAvailableSemaphores;
        std::vector<VkSemaphore>  m_renderFinishedSemaphores;
        std::vector<VkFence>      m_inFlightFences;
        std::vector<VkFence>      m_inFlightImages;

        size_t                    m_currentFrame = 0;

        SubmitCreateInfo          m_info;
};
