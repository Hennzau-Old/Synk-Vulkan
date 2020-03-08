#include <iostream>

#include "core/utils/File.h"

#include "core/Window.h"
#include "core/Instance.h"
#include "core/Surface.h"
#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"
#include "core/SwapChain.h"

#include "core/rendering/Shader.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/CommandPool.h"
#include "core/rendering/CommandBuffers.h"

#include "core/rendering/Submit.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

Window            window;
Instance          instance;
Surface           surface;
PhysicalDevice    physicalDevice;
LogicalDevice     logicalDevice;
SwapChain         swapChain;

Shader            shader;
RenderPass        renderPass;
Pipeline          pipeline;

std::vector<Framebuffer> framebuffers;

CommandPool       commandPool;
CommandBuffers    commandBuffers;

Submit            submit;

void init()
{
    Logger::init("___CORE__Components___");

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

    Surface::SurfaceCreateInfo surfaceCreateInfo  = {};
    surfaceCreateInfo.instance                    = &instance;
    surfaceCreateInfo.window                      = &window;

    if (Surface::createSurface(&surface, surfaceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createSurface failed!");
    } else
    {
        Logger::printSuccess("main::init", "createSurface succeed!");
    }

    PhysicalDevice::PhysicalDeviceCreateInfo physicalDeviceCreateInfo = {};
    physicalDeviceCreateInfo.instance                                 = &instance;
    physicalDeviceCreateInfo.surface                                  = &surface;

    if (PhysicalDevice::createPhysicalDevice(&physicalDevice, physicalDeviceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createPhysicalDevice failed!");
    } else
    {
        Logger::printSuccess("main::init", "createPhysicalDevice succeed!");
    }

    LogicalDevice::LogicalDeviceCreateInfo logicalDeviceCreateInfo  = {};
    logicalDeviceCreateInfo.instance                                = &instance;
    logicalDeviceCreateInfo.physicalDevice                          = &physicalDevice;

    if (LogicalDevice::createLogicalDevice(&logicalDevice, logicalDeviceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createLogicalDevice failed!");
    } else
    {
        Logger::printSuccess("main::init", "createLogicalDevice succeed!");
    }

    SwapChain::SwapChainCreateInfo swapChainCreateInfo  = {};
    swapChainCreateInfo.window                          = &window;
    swapChainCreateInfo.surface                         = &surface;
    swapChainCreateInfo.physicalDevice                  = &physicalDevice;
    swapChainCreateInfo.logicalDevice                   = &logicalDevice;
    swapChainCreateInfo.imageUsage                      = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (SwapChain::createSwapChain(&swapChain, swapChainCreateInfo) != 0)
    {
        Logger::printError("main::init", "createSwapChain failed!");
    } else
    {
        Logger::printSuccess("main::init", "createSwapChain succeed!");
    }

    Logger::exit("___CORE__Components___");
    Logger::init("_RENDERING_Components_");

    /* rendering */

    Shader::ShaderCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.logicalDevice            = &logicalDevice;
    shaderCreateInfo.vertexShaderCode         = File::readFile("res/shaders/vert.spv");
    shaderCreateInfo.fragmentShaderCode       = File::readFile("res/shaders/frag.spv");

    if (Shader::createShader(&shader, shaderCreateInfo) != 0)
    {
        Logger::printError("main::init", "createShader failed!");
    } else
    {
        Logger::printSuccess("main::init", "createShader succeed!");
    }

    RenderPass::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.logicalDevice                    = &logicalDevice;
    renderPassCreateInfo.swapChain                        = &swapChain;

    if (RenderPass::createRenderPass(&renderPass, renderPassCreateInfo) != 0)
    {
        Logger::printError("main::init", "createRenderPass failed!");
    } else
    {
        Logger::printSuccess("main::init", "createRenderPass ucceed!");
    }

    Pipeline::PipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.logicalDevice                = &logicalDevice;
    pipelineCreateInfo.swapChain                    = &swapChain;
    pipelineCreateInfo.shader                       = &shader;
    pipelineCreateInfo.renderPass                   = &renderPass;
    pipelineCreateInfo.topology                     = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    if (Pipeline::createPipeline(&pipeline, pipelineCreateInfo) != 0)
    {
        Logger::printError("main::init", "createPipeline failed!");
    } else
    {
        Logger::printSuccess("main::init", "createPipeline ucceed!");
    }

    Logger::init("_____FRAMEBUFFERS_____");

    framebuffers.resize(swapChain.getImageViews().size());

    Framebuffer::FramebufferCreateInfo framebufferCreateInfo  = {};
    framebufferCreateInfo.logicalDevice                       = &logicalDevice;
    framebufferCreateInfo.swapChain                           = &swapChain;
    framebufferCreateInfo.renderPass                          = &renderPass;

    for (size_t i = 0; i < swapChain.getImageViews().size(); i++)
    {
        std::vector<const VkImageView*> imageViews =
        {
            &swapChain.getImageViews()[i]
        };

        framebufferCreateInfo.imageViews = imageViews;

        if (Framebuffer::createFramebuffer(&framebuffers[i], framebufferCreateInfo) != 0)
        {
            Logger::printError("main::init", "createFramebuffer failed!");
        } else
        {
            Logger::printSuccess("main::init", "createFramebuffer[" + std::to_string(i) + "] succeed!");
        }
    }

    Logger::exit("_____FRAMEBUFFERS_____");

    CommandPool::CommandPoolCreateInfo commandPoolCreateInfo  = {};
    commandPoolCreateInfo.physicalDevice                      = &physicalDevice;
    commandPoolCreateInfo.logicalDevice                       = &logicalDevice;

    if (CommandPool::createCommandPool(&commandPool, commandPoolCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandPool failed!");
    } else
    {
        Logger::printSuccess("main::init", "createCommandPool succeed!");
    }

    CommandBuffers::CommandBuffersCreateInfo commandBuffersCreateInfo = {};
    commandBuffersCreateInfo.framebuffers                             = framebuffers;
    commandBuffersCreateInfo.logicalDevice                            = &logicalDevice;
    commandBuffersCreateInfo.swapChain                                = &swapChain;
    commandBuffersCreateInfo.renderPass                               = &renderPass;
    commandBuffersCreateInfo.pipeline                                 = &pipeline;
    commandBuffersCreateInfo.commandPool                              = &commandPool;

    if (CommandBuffers::createCommandBuffers(&commandBuffers, commandBuffersCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandBuffers failed!");
    } else
    {
        Logger::printSuccess("main::init", "createCommandBuffers succeed!");
    }

    commandBuffers.beginCommandBuffers();
        commandBuffers.beginRenderPass();

            commandBuffers.bindPipeline();
            commandBuffers.draw();

        commandBuffers.endRenderPass();
    commandBuffers.endCommandBuffers();

    Submit::SubmitCreateInfo submitCreateInfo = {};
    submitCreateInfo.logicalDevice            = &logicalDevice;
    submitCreateInfo.swapChain                = &swapChain;

    if(Submit::createSubmit(&submit, submitCreateInfo) != 0)
    {
        Logger::printError("main::init", "createSubmit failed!");
    } else
    {
        Logger::printSuccess("main::init", "createSubmit succeed");
    }

    Logger::exit("_RENDERING_Components_");
}

void update()
{

}

void render()
{
    window.update();

    submit.submit(commandBuffers);
}

void clean()
{
    commandPool.clean();

    for (auto framebuffer : framebuffers)
    {
        framebuffer.clean();
    }

    pipeline.clean();
    renderPass.clean();
    shader.clean();

    swapChain.clean();
    logicalDevice.clean();
    surface.clean();
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

    vkDeviceWaitIdle(logicalDevice.getLogicalDevice());

    clean();

    return 0;
}
