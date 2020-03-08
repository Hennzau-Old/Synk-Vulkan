#include "core/SwapChain.h"

SwapChain::SwapChain()
{

}

SwapChain::~SwapChain()
{

}

void SwapChain::clean()
{
    for (auto imageView : m_imageViews)
    {
        vkDestroyImageView(m_info.logicalDevice->getLogicalDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_info.logicalDevice->getLogicalDevice(), m_swapChain, nullptr);
}

void SwapChain::setData(const SwapChainCreateInfo& createInfo)
{
    m_info = createInfo;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    } else
    {
        VkExtent2D actualExtent =
        {
            m_info.window->getInfo().width,
            m_info.window->getInfo().height
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

int SwapChain::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapCainSupport(m_info.physicalDevice->getPhysicalDevice(), m_info.surface->getSurface());

    m_surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    m_presentMode   = chooseSwapPresentMode(swapChainSupport.presentModes);
    m_extent        = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface                  = m_info.surface->getSurface();
    createInfo.minImageCount            = imageCount;
    createInfo.imageFormat              = m_surfaceFormat.format;
    createInfo.imageColorSpace          = m_surfaceFormat.colorSpace;
    createInfo.imageExtent              = m_extent;
    createInfo.imageArrayLayers         = 1;
    createInfo.imageUsage               = m_info.imageUsage;

    auto indices  = m_info.physicalDevice->getQueueFamilies();

    uint32_t queueFamilyIndices[] =
    {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode       = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount  = 2;
        createInfo.pQueueFamilyIndices    = queueFamilyIndices;
    } else
    {
        createInfo.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount  = 0;
        createInfo.pQueueFamilyIndices    = nullptr;
    }

    createInfo.preTransform     = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode      = m_presentMode;
    createInfo.clipped          = VK_TRUE;
    createInfo.oldSwapchain     = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_info.logicalDevice->getLogicalDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        Logger::printError("SwapChain::createSwapChain", "vkCreateSwapchainKHR failed!");

        return 1;
    }

    vkGetSwapchainImagesKHR(m_info.logicalDevice->getLogicalDevice(), m_swapChain, &imageCount, nullptr);
    m_images.resize(imageCount);

    vkGetSwapchainImagesKHR(m_info.logicalDevice->getLogicalDevice(), m_swapChain, &imageCount, m_images.data());

    m_imageFormat = m_surfaceFormat.format;

    return 0;
}

int SwapChain::createImageViews()
{
    m_imageViews.resize(m_images.size());

    VkImageViewCreateInfo createInfo  = {};
    for (size_t i = 0; i < m_images.size(); i++)
    {
        createInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                  = m_images[i];
        createInfo.viewType               = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                 = m_imageFormat;
        createInfo.components.r           = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g           = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b           = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a           = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel    = 0;
        createInfo.subresourceRange.levelCount      = 1;
        createInfo.subresourceRange.baseArrayLayer  = 0;
        createInfo.subresourceRange.layerCount      = 1;

        if (vkCreateImageView(m_info.logicalDevice->getLogicalDevice(), &createInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS)
        {
            Logger::printError("SwapChain::createImageViews", "vkCreateImageView failed!");
        }
    }

    return 0;
}

int SwapChain::createSwapChain(SwapChain* swapChain, const SwapChainCreateInfo& createInfo)
{
    swapChain->setData(createInfo);

    return swapChain->createSwapChain() + swapChain->createImageViews();
}

SwapChain::SwapChainSupportDetails SwapChain::querySwapCainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
    SwapChain::SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    } else
    {
        Logger::printError("SwapChain::querySwapCainSupport", "vkGetPhysicalDeviceSurfaceFormatsKHR failed!");
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    } else
    {
        Logger::printError("SwapChain::querySwapCainSupport", "vkGetPhysicalDeviceSurfacePresentModesKHR failed!");
    }

    return details;
}

SwapChain::SwapChainCreateInfo& SwapChain::getInfo()
{
    return m_info;
}
