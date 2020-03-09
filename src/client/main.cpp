#include <iostream>

#include "core/utils/File.h"

#include "core/CoreComponents.h"

#include "core/rendering/Shader.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/CommandPool.h"
#include "core/rendering/CommandBuffers.h"

#include "core/rendering/Submit.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

CoreComponents    coreComponents;

Shader            shader;
RenderPass        renderPass;
Pipeline          pipeline;

std::vector<Framebuffer> framebuffers;

CommandPool       commandPool;
CommandBuffers    commandBuffers;

Submit            submit;

void init()
{
    Window::WindowCreateInfo windowCreateInfo = {};
    windowCreateInfo.width                    = 1280;
    windowCreateInfo.height                   = 720;
    windowCreateInfo.resizable                = GLFW_FALSE;
    windowCreateInfo.title                    = "Synk Vulkan Engine";

    CoreComponents::CoreComponentsCreateInfo coreComponentsCreateInfo = {};
    coreComponentsCreateInfo.windowCreateInfo                         = windowCreateInfo;
    coreComponentsCreateInfo.imageUsage                               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (CoreComponents::createCoreComponents(&coreComponents, coreComponentsCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCoreComponents failed!");
    }

    Logger::init("_RENDERING_Components_");

    /* rendering */

    Shader::ShaderCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.logicalDevice            = &coreComponents.getLogicalDevice();
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
    renderPassCreateInfo.logicalDevice                    = &coreComponents.getLogicalDevice();
    renderPassCreateInfo.swapChain                        = &coreComponents.getSwapChain();

    if (RenderPass::createRenderPass(&renderPass, renderPassCreateInfo) != 0)
    {
        Logger::printError("main::init", "createRenderPass failed!");
    } else
    {
        Logger::printSuccess("main::init", "createRenderPass ucceed!");
    }

    Pipeline::PipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.logicalDevice                = &coreComponents.getLogicalDevice();
    pipelineCreateInfo.swapChain                    = &coreComponents.getSwapChain();
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

    framebuffers.resize(coreComponents.getSwapChain().getImageViews().size());

    Framebuffer::FramebufferCreateInfo framebufferCreateInfo  = {};
    framebufferCreateInfo.logicalDevice                       = &coreComponents.getLogicalDevice();
    framebufferCreateInfo.swapChain                           = &coreComponents.getSwapChain();
    framebufferCreateInfo.renderPass                          = &renderPass;

    for (size_t i = 0; i < coreComponents.getSwapChain().getImageViews().size(); i++)
    {
        std::vector<const VkImageView*> imageViews =
        {
            &coreComponents.getSwapChain().getImageViews()[i]
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
    commandPoolCreateInfo.physicalDevice                      = &coreComponents.getPhysicalDevice();
    commandPoolCreateInfo.logicalDevice                       = &coreComponents.getLogicalDevice();

    if (CommandPool::createCommandPool(&commandPool, commandPoolCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandPool failed!");
    } else
    {
        Logger::printSuccess("main::init", "createCommandPool succeed!");
    }

    CommandBuffers::CommandBuffersCreateInfo commandBuffersCreateInfo = {};
    commandBuffersCreateInfo.framebuffers                             = framebuffers;
    commandBuffersCreateInfo.logicalDevice                            = &coreComponents.getLogicalDevice();
    commandBuffersCreateInfo.swapChain                                = &coreComponents.getSwapChain();
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
    submitCreateInfo.logicalDevice            = &coreComponents.getLogicalDevice();
    submitCreateInfo.swapChain                = &coreComponents.getSwapChain();

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
    coreComponents.getWindow().update();

    submit.submit(commandBuffers);
}

void clean()
{
    submit.clean();

    commandPool.clean();

    for (auto framebuffer : framebuffers)
    {
        framebuffer.clean();
    }

    pipeline.clean();
    renderPass.clean();
    shader.clean();

    coreComponents.clean();
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

    while(!coreComponents.getWindow().isClosed())
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

    vkDeviceWaitIdle(coreComponents.getLogicalDevice().getLogicalDevice());

    clean();

    return 0;
}
