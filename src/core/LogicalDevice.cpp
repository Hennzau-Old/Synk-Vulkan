#include "core/LogicalDevice.h"

LogicalDevice::LogicalDevice()
{

}

LogicalDevice::~LogicalDevice()
{

}

void LogicalDevice::clean()
{
    vkDestroyDevice(m_logicalDevice, nullptr);

    Logger::printInfo("LogicalDevice::clean", "vkDestroyDevice!");
}

void LogicalDevice::setData(const LogicalDeviceCreateInfo& createInfo)
{
    m_info = createInfo;
}

void LogicalDevice::wait() const
{
    vkDeviceWaitIdle(m_logicalDevice);
}

int LogicalDevice::createLogicalDevice()
{
    auto indices = m_info.physicalDevice->getQueueFamilies();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies =
    {
        indices.graphicsFamily.value(),
        indices.presentFamily.value(),
    };

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex        = queueFamily;
        queueCreateInfo.queueCount              = 1;
        queueCreateInfo.pQueuePriorities        = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo           = {};
    createInfo.sType                        = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount         = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos            = queueCreateInfos.data();
    createInfo.pEnabledFeatures             = &deviceFeatures;
    createInfo.enabledExtensionCount        = static_cast<uint32_t>(m_info.physicalDevice->deviceExtensions.size());
    createInfo.ppEnabledExtensionNames      = m_info.physicalDevice->deviceExtensions.data();

    if (Instance::enabledValidationLayers)
    {
        createInfo.enabledLayerCount        = static_cast<uint32_t>(m_info.instance->validationLayers.size());
        createInfo.ppEnabledLayerNames      = m_info.instance->validationLayers.data();
    } else
    {
        createInfo.enabledLayerCount        = 0;
    }

    if (vkCreateDevice(m_info.physicalDevice->getPhysicalDevice(), &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        Logger::printError("LogicalDevice::createLogicalDevice", "vkCreateDevice failed!");

        return 1;
    }

    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);

    return 0;
}

int LogicalDevice::createLogicalDevice(LogicalDevice* logicalDevice, const LogicalDeviceCreateInfo& createInfo)
{
    logicalDevice->setData(createInfo);

    return logicalDevice->createLogicalDevice();
}

LogicalDevice::LogicalDeviceCreateInfo& LogicalDevice::getInfo()
{
    return m_info;
}
