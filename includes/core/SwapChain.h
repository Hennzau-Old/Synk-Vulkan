#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/Window.h"
#include "core/Surface.h"
#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

class SwapChain
{
    public:

        /* static variables */



        /* structure */

        struct SwapChainCreateInfo
        {
            Window*                         window          = nullptr;
            Surface*                        surface         = nullptr;
            PhysicalDevice*                 physicalDevice  = nullptr;
            LogicalDevice*                  logicalDevice   = nullptr;

            VkImageUsageFlags               imageUsage;
        };

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR        capabilities;

            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR>   presentModes;
        };

        /* functions */

        SwapChain();
        ~SwapChain();

        void                            clean();
        void                            setData(const SwapChainCreateInfo& createInfo);

        static int                      createSwapChain(SwapChain* swapChain, const SwapChainCreateInfo& createInfo);
        static SwapChainSupportDetails  querySwapCainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

        inline VkSwapchainKHR&          getSwapChain() { return m_swapChain; }

        inline VkSurfaceFormatKHR&      getSurfaceFormat() { return m_surfaceFormat; }
        inline VkFormat&                getImageFormat  () { return m_imageFormat; }
        inline VkPresentModeKHR&        getPresentMode  () { return m_presentMode; }
        inline VkExtent2D&              getExtent       () { return m_extent; }

        inline std::vector<VkImage>&      getImages     () { return m_images; }
        inline std::vector<VkImageView>&  getImageViews () { return m_imageViews; }

        SwapChainCreateInfo&            getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                             createSwapChain();
        int                             createImageViews();

        VkSurfaceFormatKHR              chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR                chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D                      chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        /* variables */

        VkSwapchainKHR                  m_swapChain;

        VkSurfaceFormatKHR              m_surfaceFormat;
        VkFormat                        m_imageFormat;
        VkPresentModeKHR                m_presentMode;
        VkExtent2D                      m_extent;

        std::vector<VkImage>            m_images;
        std::vector<VkImageView>        m_imageViews;

        SwapChainCreateInfo             m_info;
};
