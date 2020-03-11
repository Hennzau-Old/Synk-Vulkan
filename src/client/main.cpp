#include <iostream>

#include "core/utils/File.h"

#include "core/CoreComponents.h"

#include "core/rendering/Shader.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/CommandPool.h"
#include "core/rendering/CommandBuffers.h"

#include "core/rendering/WindowResizeManager.h"
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

WindowResizeManager windowResizeManager;
Submit            submit;

void drawAll(CommandBuffers* commandBuffers)
{
    for (size_t i = 0; i < framebuffers.size(); i++)
    {
        commandBuffers->beginCommandBuffers(i);
            commandBuffers->beginRenderPass(i, renderPass);

                commandBuffers->bindPipeline(i, pipeline);
                commandBuffers->draw(i);

            commandBuffers->endRenderPass(i);
        commandBuffers->endCommandBuffers(i);
    }
}

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

    RenderPass::RenderPassAttachmentsInfo renderPassAttachmentsInfo = {};
    renderPassAttachmentsInfo.format                                = coreComponents.getSwapChain().getImageFormat();
    renderPassAttachmentsInfo.samples                               = VK_SAMPLE_COUNT_1_BIT;
    renderPassAttachmentsInfo.loadOp                                = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassAttachmentsInfo.storeOp                               = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassAttachmentsInfo.stencilLoadOp                         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassAttachmentsInfo.stencilStoreOp                        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassAttachmentsInfo.initialLayout                         = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassAttachmentsInfo.finalLayout                           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    RenderPass::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.logicalDevice                    = &coreComponents.getLogicalDevice();
    renderPassCreateInfo.attachmentsInfo                  = renderPassAttachmentsInfo;

    if (RenderPass::createRenderPass(&renderPass, renderPassCreateInfo) != 0)
    {
        Logger::printError("main::init", "createRenderPass failed!");
    } else
    {
        Logger::printSuccess("main::init", "createRenderPass succeed!");
    }

    Pipeline::PipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.logicalDevice                = &coreComponents.getLogicalDevice();
    pipelineCreateInfo.swapChain                    = &coreComponents.getSwapChain();
    pipelineCreateInfo.shader                       = &shader;
    pipelineCreateInfo.renderPass                   = &renderPass;

    if (Pipeline::createPipeline(&pipeline, pipelineCreateInfo) != 0)
    {
        Logger::printError("main::init", "createPipeline failed!");
    } else
    {
        Logger::printSuccess("main::init", "createPipeline succeed!");
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
    commandBuffersCreateInfo.framebuffers                             = &framebuffers;
    commandBuffersCreateInfo.logicalDevice                            = &coreComponents.getLogicalDevice();
    commandBuffersCreateInfo.swapChain                                = &coreComponents.getSwapChain();
    commandBuffersCreateInfo.commandPool                              = &commandPool;

    if (CommandBuffers::createCommandBuffers(&commandBuffers, commandBuffersCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandBuffers failed!");
    } else
    {
        Logger::printSuccess("main::init", "createCommandBuffers succeed!");
    }

    drawAll(&commandBuffers);

    WindowResizeManager::WindowResizeManagerCreateInfo windowResizeManagerCreateInfo = {};
    windowResizeManagerCreateInfo.logicalDevice     = &coreComponents.getLogicalDevice();
    windowResizeManagerCreateInfo.swapChain         = &coreComponents.getSwapChain();
    windowResizeManagerCreateInfo.renderPass        = &renderPass;
    windowResizeManagerCreateInfo.pipeline          = &pipeline;
    windowResizeManagerCreateInfo.commandBuffers    = &commandBuffers;
    windowResizeManagerCreateInfo.framebuffers      = &framebuffers;
    windowResizeManagerCreateInfo.drawFunction      = drawAll;

    if (WindowResizeManager::createWindowResizeManager(&windowResizeManager, windowResizeManagerCreateInfo) != 0)
    {
        Logger::printError("main::init", "createWindowResizeManager failed!");
    } else
    {
        Logger::printSuccess("main::init", "createWindowResizeManager succeed!");
    }

    Submit::SubmitCreateInfo submitCreateInfo = {};
    submitCreateInfo.window                   = &coreComponents.getWindow();
    submitCreateInfo.logicalDevice            = &coreComponents.getLogicalDevice();
    submitCreateInfo.swapChain                = &coreComponents.getSwapChain();
    submitCreateInfo.windowResizeManager      = &windowResizeManager;

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
    Logger::init("___CLEAN__RESOURCES___");
    Logger::init("___CLEAN__RENDERING___");

    submit.clean();

    commandBuffers.clean();
    commandPool.clean();

    for (auto framebuffer : framebuffers)
    {
        framebuffer.clean();
    }

    pipeline.clean();
    renderPass.clean();
    shader.clean();

    Logger::exit("___CLEAN__RENDERING___");

    coreComponents.clean();

    Logger::exit("___CLEAN__RESOURCES___");
}

int main()
{
    init();

    Logger::init("_____GAME__ENGINE_____");

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

    Logger::exit("_____GAME__ENGINE_____");

    vkDeviceWaitIdle(coreComponents.getLogicalDevice().getLogicalDevice());

    clean();

    return 0;
}
