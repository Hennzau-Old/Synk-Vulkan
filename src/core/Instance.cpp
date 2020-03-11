#include "core/Instance.h"

Instance::Instance()
{

}

Instance::~Instance()
{

}

void Instance::clean()
{
    vkDestroyInstance(m_instance, nullptr);

    Logger::printInfo("Instance::clean", "vkDestroyInstance!");
}

void Instance::setData(const InstanceCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Instance::createInstance()
{
    if (enabledValidationLayers && !checkValidationLayerSupport())
    {
        Logger::printError("Instance::createInstance", "validatioinLayers are enabled but not supported!");
    }

    VkApplicationInfo appInfo   = {};
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName    = m_info.appName.c_str();
    appInfo.applicationVersion  = m_info.appVersion;
    appInfo.pEngineName         = m_info.engineName.c_str();
    appInfo.engineVersion       = m_info.engineVersion;
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    uint32_t extensionCount = 0;
    const char** extensions = getExtensions(extensionCount);

    VkInstanceCreateInfo createInfo     = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo         = &appInfo;
    createInfo.enabledExtensionCount    = extensionCount;
    createInfo.ppEnabledExtensionNames  = extensions;

    if (enabledValidationLayers)
    {
        createInfo.enabledLayerCount    = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames  = validationLayers.data();
    } else
    {
        createInfo.enabledLayerCount    = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        Logger::printError("Instance::createInstance", "vkCreateInstance failed!");

        return 1;
    }

    return 0;
}

const char** Instance::getExtensions(uint32_t& extensionCount)
{
    return glfwGetRequiredInstanceExtensions(&extensionCount);;
}

bool Instance::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

int Instance::createInstance(Instance* instance, const InstanceCreateInfo& createInfo)
{
    instance->setData(createInfo);

    return instance->createInstance();
}

Instance::InstanceCreateInfo& Instance::getInfo()
{
  return m_info;
}
