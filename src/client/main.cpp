#include <iostream>

#include "core/Window.h"
#include "core/Instance.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

Window            window;
Instance          instance;

void init()
{
    /* window creation */

    Window::WindowCreateInfo windowCreateInfo = {};
    windowCreateInfo.width                    = 1280;
    windowCreateInfo.height                   = 720;
    windowCreateInfo.resizable                = GLFW_FALSE;
    windowCreateInfo.title                    = "Synk Vulkan Engine";

    if (Window::createWindow(&window, windowCreateInfo) != 0)
    {
        Logger::printError("main::init", "createWindow failed!");
    } else
    {
        Logger::printSuccess("main::init", "createWindow succeed!");
    }

    Instance::InstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.appName                      = window.getTitle();
    instanceCreateInfo.appVersion                   = VK_MAKE_VERSION(0, 0, 1);
    instanceCreateInfo.engineName                   = "No Engine";
    instanceCreateInfo.engineVersion                = VK_MAKE_VERSION(0, 0, 1);

    if (Instance::createInstance(&instance, instanceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createInstance failed!");
    } else
    {
        Logger::printSuccess("main::init", "createInstance succeed!");
    }
}

void update()
{

}

void render()
{
    window.update();
}

void clean()
{
    instance.clean();
    window.clean();
}

int main()
{
    init();

    double  tickTime    = 1.0 / TICK_CAP;
    double  renderTime  = 1.0 / FRAME_CAP;

    double  lastTickTime    = glfwGetTime();
    double  timer = lastTickTime;
    double  lastRenderTime  = glfwGetTime();
    double  deltaTime       = 0,
            nowTime         = 0;

    int     frames          = 0,
            ticks           = 0;

    while(!window.isClosed())
    {
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTickTime) / tickTime;
        lastTickTime = nowTime;

        while(deltaTime >= 1.0)
        {
            ticks++;
            update();
            deltaTime--;
        }

        if(nowTime - lastRenderTime >= renderTime)
        {
            lastRenderTime = nowTime;
            render();
            frames++;
        }

        if(glfwGetTime() - timer > 1.0)
        {
            timer++;

            Logger::printText("main", "Synk - Vulkan Engine : FPS = " + std::to_string(frames) + " | TPS = " + std::to_string(ticks));

            frames = 0;
            ticks = 0;
        }
    }

    clean();

    return 0;
}
