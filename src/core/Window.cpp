#include "core/Window.h"

Window::Window()
{

}

Window::~Window()
{

}

void Window::clean()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::setData(const WindowCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Window::createWindow()
{
    if (!glfwInit())
    {
        Logger::printError("Window::createWindow", "glfwInit failed!");

        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, isResizable());

    m_window = glfwCreateWindow(getWidth(), getHeight(), getTitle().c_str(), nullptr, nullptr);

    if (!m_window)
    {
        Logger::printError("Window::createWindow", "glfwCreateWindow failed!");

        return 1;
    }

    return 0;
}

void Window::update()
{
    glfwPollEvents();
}

int Window::createWindow(Window* window, const WindowCreateInfo& createInfo)
{
    window->setData(createInfo);

    return window->createWindow();
}

bool Window::isClosed() const
{
    return glfwWindowShouldClose(m_window);
}

Window::WindowCreateInfo& Window::getInfo()
{
    return m_info;
}
