#include "core/CoreComponents.h"

CoreComponents::CoreComponents()
{

}

CoreComponents::~CoreComponents()
{

}

void CoreComponents::clean()
{
    Logger::init("___CLEAN_____CORE_____");

    m_swapChain.clean();
    m_logicalDevice.clean();
    m_surface.clean();
    m_instance.clean();
    m_window.clean();

    Logger::exit("___CLEAN_____CORE_____");
}

void CoreComponents::setData(const CoreComponentsCreateInfo& createInfo)
{
    m_info = createInfo;
}

int CoreComponents::createWindow()
{
    if (Window::createWindow(&m_window, m_info.windowCreateInfo) != 0)
    {
        Logger::printError("coreComponents::createWindow", "createWindow failed!");
    } else
    {
        Logger::printSuccess("coreComponents::createWindow", "createWindow succeed!");
    }

    return 0;
}

int CoreComponents::createInstance()
{
    Instance::InstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.appName                      = m_window.getTitle();
    instanceCreateInfo.appVersion                   = VK_MAKE_VERSION(0, 0, 1);
    instanceCreateInfo.engineName                   = "No Engine";
    instanceCreateInfo.engineVersion                = VK_MAKE_VERSION(0, 0, 1);

    if (Instance::createInstance(&m_instance, instanceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createInstance", "createInstance failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createInstance", "createInstance succeed!");
    }

    return 0;
}

int CoreComponents::createSurface()
{
    Surface::SurfaceCreateInfo surfaceCreateInfo  = {};
    surfaceCreateInfo.instance                    = &m_instance;
    surfaceCreateInfo.window                      = &m_window;

    if (Surface::createSurface(&m_surface, surfaceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createSurface", "createSurface failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createSurface", "createSurface succeed!");
    }

    return 0;
}

int CoreComponents::createPhysicalDevice()
{
    PhysicalDevice::PhysicalDeviceCreateInfo physicalDeviceCreateInfo = {};
    physicalDeviceCreateInfo.instance                                 = &m_instance;
    physicalDeviceCreateInfo.surface                                  = &m_surface;

    if (PhysicalDevice::createPhysicalDevice(&m_physicalDevice, physicalDeviceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createPhysicalDevice", "createPhysicalDevice failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createPhysicalDevice", "createPhysicalDevice succeed!");
    }

    return 0;
}

int CoreComponents::createLogicalDevice()
{
    LogicalDevice::LogicalDeviceCreateInfo logicalDeviceCreateInfo  = {};
    logicalDeviceCreateInfo.instance                                = &m_instance;
    logicalDeviceCreateInfo.physicalDevice                          = &m_physicalDevice;

    if (LogicalDevice::createLogicalDevice(&m_logicalDevice, logicalDeviceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createLogicalDevice", "createLogicalDevice failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createLogicalDevice", "createLogicalDevice succeed!");
    }

    return 0;
}

int CoreComponents::createSwapChain()
{
    SwapChain::SwapChainCreateInfo swapChainCreateInfo  = {};
    swapChainCreateInfo.window                          = &m_window;
    swapChainCreateInfo.surface                         = &m_surface;
    swapChainCreateInfo.physicalDevice                  = &m_physicalDevice;
    swapChainCreateInfo.logicalDevice                   = &m_logicalDevice;
    swapChainCreateInfo.imageUsage                      = m_info.imageUsage;

    if (SwapChain::createSwapChain(&m_swapChain, swapChainCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createSwapChain", "createSwapChain failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createSwapChain", "createSwapChain succeed!");
    }

    return 0;
}

int CoreComponents::endCreation()
{
    Logger::exit("___CORE__Components___");

    return 0;
}

int CoreComponents::createCoreComponents(CoreComponents* coreComponents, const CoreComponentsCreateInfo& createInfo)
{
    coreComponents->setData(createInfo);

    Logger::init("___CORE__Components___");

    return  coreComponents->createWindow() +
            coreComponents->createInstance() +
            coreComponents->createSurface() +
            coreComponents->createPhysicalDevice() +
            coreComponents->createLogicalDevice() +
            coreComponents->createSwapChain() +
            coreComponents->endCreation();
}

CoreComponents::CoreComponentsCreateInfo& CoreComponents::getInfo()
{
  return m_info;
}
