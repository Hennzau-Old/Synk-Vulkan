#include "core/rendering/Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::clean()
{
    Logger::init("___CLEAN__RENDERING___");

    m_submit.clean();

    m_commandBuffers.clean();
    m_commandPool.clean();

    for (auto framebuffer : m_framebuffers)
    {
        framebuffer.clean();
    }

    m_pipeline.clean();
    m_renderPass.clean();
    m_shader.clean();

    Logger::exit("___CLEAN__RENDERING___");
}

void Scene::setData(const SceneCreateInfo& createInfo)
{
    m_info = createInfo;
}

void Scene::render()
{
    m_submit.submit(m_commandBuffers);
}

int Scene::createShaders()
{
    m_info.shaderCreateInfo.logicalDevice = &m_info.coreComponents->getLogicalDevice();

    if (Shader::createShader(&m_shader, m_info.shaderCreateInfo) != 0)
    {
        Logger::printError("Scene::createShaders", "createShader failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createShaders", "createShader succeed!");
    }

    return 0;
}

int Scene::createRenderPass()
{
    RenderPass::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.logicalDevice                    = &m_info.coreComponents->getLogicalDevice();
    renderPassCreateInfo.attachmentsInfo                  = m_info.renderPassAttachmentsInfo;

    if (RenderPass::createRenderPass(&m_renderPass, renderPassCreateInfo) != 0)
    {
        Logger::printError("Scene::createRenderPass", "createRenderPass failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createRenderPass", "createRenderPass succeed!");
    }

    return 0;
}

int Scene::createPipeline()
{
    Pipeline::PipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.logicalDevice                = &m_info.coreComponents->getLogicalDevice();
    pipelineCreateInfo.swapChain                    = &m_info.coreComponents->getSwapChain();
    pipelineCreateInfo.shader                       = &m_shader;
    pipelineCreateInfo.renderPass                   = &m_renderPass;
    pipelineCreateInfo.rasterizationInfo            = m_info.rasterizationInfo;
    pipelineCreateInfo.vertexInputInfo              = m_info.vertexInputInfo;

    if (Pipeline::createPipeline(&m_pipeline, pipelineCreateInfo) != 0)
    {
        Logger::printError("Scene::createPipeline", "createPipeline failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createPipeline", "createPipeline succeed!");
    }

    return 0;
}

int Scene::createFramebuffers()
{
    Logger::init("_____FRAMEBUFFERS_____");

    m_framebuffers.resize(m_info.coreComponents->getSwapChain().getImageViews().size());

    Framebuffer::FramebufferCreateInfo framebufferCreateInfo  = {};
    framebufferCreateInfo.logicalDevice                       = &m_info.coreComponents->getLogicalDevice();
    framebufferCreateInfo.swapChain                           = &m_info.coreComponents->getSwapChain();
    framebufferCreateInfo.renderPass                          = &m_renderPass;

    for (size_t i = 0; i < m_info.coreComponents->getSwapChain().getImageViews().size(); i++)
    {
        std::vector<const VkImageView*> imageViews =
        {
            &m_info.coreComponents->getSwapChain().getImageViews()[i]
        };

        framebufferCreateInfo.imageViews = imageViews;

        if (Framebuffer::createFramebuffer(&m_framebuffers[i], framebufferCreateInfo) != 0)
        {
            Logger::printError("Scene::createFramebuffers", "createFramebuffer failed!");

            return 1;
        } else
        {
            Logger::printSuccess("Scene::createFramebuffers", "createFramebuffer[" + std::to_string(i) + "] succeed!");
        }
    }

    Logger::exit("_____FRAMEBUFFERS_____");

    return 0;
}

int Scene::createCommandPool()
{
    CommandPool::CommandPoolCreateInfo commandPoolCreateInfo  = {};
    commandPoolCreateInfo.physicalDevice                      = &m_info.coreComponents->getPhysicalDevice();
    commandPoolCreateInfo.logicalDevice                       = &m_info.coreComponents->getLogicalDevice();
    commandPoolCreateInfo.queueFamilyIndex                    = m_info.coreComponents->getPhysicalDevice().getQueueFamilies().graphicsFamily.value();

    if (CommandPool::createCommandPool(&m_commandPool, commandPoolCreateInfo) != 0)
    {
        Logger::printError("Scene::createCommandPool", "createCommandPool failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createCommandPool", "createCommandPool succeed!");
    }

    return 0;
}

int Scene::createCommandBuffers()
{
    CommandBuffers::CommandBuffersCreateInfo commandBuffersCreateInfo = {};
    commandBuffersCreateInfo.framebuffers                             = &m_framebuffers;
    commandBuffersCreateInfo.logicalDevice                            = &m_info.coreComponents->getLogicalDevice();
    commandBuffersCreateInfo.swapChain                                = &m_info.coreComponents->getSwapChain();
    commandBuffersCreateInfo.commandPool                              = &m_commandPool;

    if (CommandBuffers::createCommandBuffers(&m_commandBuffers, commandBuffersCreateInfo) != 0)
    {
        Logger::printError("Scene::createCommandBuffers", "createCommandBuffers failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createCommandBuffers", "createCommandBuffers succeed!");
    }

    m_info.drawFunction(&m_commandBuffers);

    return 0;
}

int Scene::createWindowResizeManager()
{
    WindowResizeManager::WindowResizeManagerCreateInfo windowResizeManagerCreateInfo = {};
    windowResizeManagerCreateInfo.logicalDevice     = &m_info.coreComponents->getLogicalDevice();
    windowResizeManagerCreateInfo.swapChain         = &m_info.coreComponents->getSwapChain();
    windowResizeManagerCreateInfo.renderPass        = &m_renderPass;
    windowResizeManagerCreateInfo.pipeline          = &m_pipeline;
    windowResizeManagerCreateInfo.commandBuffers    = &m_commandBuffers;
    windowResizeManagerCreateInfo.framebuffers      = &m_framebuffers;
    windowResizeManagerCreateInfo.drawFunction      = m_info.drawFunction;

    if (WindowResizeManager::createWindowResizeManager(&m_windowResizeManager, windowResizeManagerCreateInfo) != 0)
    {
        Logger::printError("Scene::createWindowResizeManager", "createWindowResizeManager failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createWindowResizeManager", "createWindowResizeManager succeed!");
    }

    return 0;
}

int Scene::createSubmit()
{
    Submit::SubmitCreateInfo submitCreateInfo = {};
    submitCreateInfo.window                   = &m_info.coreComponents->getWindow();
    submitCreateInfo.logicalDevice            = &m_info.coreComponents->getLogicalDevice();
    submitCreateInfo.swapChain                = &m_info.coreComponents->getSwapChain();
    submitCreateInfo.windowResizeManager      = &m_windowResizeManager;

    if(Submit::createSubmit(&m_submit, submitCreateInfo) != 0)
    {
        Logger::printError("Scene::createSubmit", "createSubmit failed!");
    } else
    {
        Logger::printSuccess("Scene::createSubmit", "createSubmit succeed");
    }

    return 0;
}

int Scene::endCreation()
{
    Logger::exit("_RENDERING_Components_");

    return 0;
}

int Scene::createScene(Scene* scene, const SceneCreateInfo& createInfo)
{
    scene->setData(createInfo);

    Logger::init("_RENDERING_Components_");

    return  scene->createShaders() +
            scene->createRenderPass() +
            scene->createPipeline() +
            scene->createFramebuffers() +
            scene->createCommandPool() +
            scene->createCommandBuffers() +
            scene->createWindowResizeManager() +
            scene->createSubmit() +
            scene->endCreation();
}

Scene::SceneCreateInfo& Scene::getInfo()
{
  return m_info;
}
