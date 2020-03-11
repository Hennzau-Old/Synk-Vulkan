#include "core/Surface.h"

Surface::Surface()
{

}

Surface::~Surface()
{

}

void Surface::clean()
{
    vkDestroySurfaceKHR(m_info.instance->getInstance(), m_surface, nullptr);

    Logger::printInfo("Surface::clean", "vkDestroySurfaceKHR!");
}

void Surface::setData(const SurfaceCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Surface::createSurface()
{
    if (glfwCreateWindowSurface(m_info.instance->getInstance(), m_info.window->getGLFWWindow(), nullptr, &m_surface) != VK_SUCCESS)
    {
        Logger::printError("Surface::createSurface", "glfwCreateWindowSurface failed!");

        return 1;
    }

    return 0;
}

int Surface::createSurface(Surface* surface, const SurfaceCreateInfo& createInfo)
{
    surface->setData(createInfo);

    return surface->createSurface();
}

Surface::SurfaceCreateInfo& Surface::getInfo()
{
  return m_info;
}
