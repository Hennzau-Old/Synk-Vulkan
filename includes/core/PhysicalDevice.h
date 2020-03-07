#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <optional>
#include <vector>
#include <map>
#include <set>

#include "core/utils/logs/Logger.h"

#include "core/Instance.h"
#include "core/Surface.h"

class PhysicalDevice
{
    public:

        /* static / const variables */

        const std::vector<const char*> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        /* structure */

        struct PhysicalDeviceCreateInfo
        {
            Instance*               instance  = nullptr;
            Surface*                surface   = nullptr;
        };

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete()
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        /* functions */

        PhysicalDevice();
        ~PhysicalDevice();

        void                        clean();
        void                        setData(const PhysicalDeviceCreateInfo& createInfo);

        static int                  createPhysicalDevice(PhysicalDevice* physicalDevice, const PhysicalDeviceCreateInfo& createInfo);

        inline VkPhysicalDevice&    getPhysicalDevice () { return m_physicalDevice; }
        inline QueueFamilyIndices&  getQueueFamilies  () { return m_queueFamily; }

        PhysicalDeviceCreateInfo&   getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        bool                        checkDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice);
        QueueFamilyIndices          findQueueFamilies(const VkPhysicalDevice& physicalDevice);
        uint32_t                    ratePhysicalDeviceSuitability(const VkPhysicalDevice& physicalDevice);
        int                         createPhysicalDevice();

        /* variables */

        VkPhysicalDevice            m_physicalDevice  = VK_NULL_HANDLE;
        QueueFamilyIndices          m_queueFamily;

        PhysicalDeviceCreateInfo    m_info;
};
