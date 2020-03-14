#include <iostream>

#include "core/maths/maths.h"
#include "core/utils/File.h"

#include "core/CoreComponents.h"

#include "core/rendering/Mesh.h"
#include "core/rendering/Scene.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

CoreComponents    coreComponents;

Mesh              mesh;
Scene             scene;

void drawAll(CommandBuffers* commandBuffers)
{
    std::vector<VkBuffer> vertexBuffers =
    {
        mesh.getVertexBuffer()
    };

    for (size_t i = 0; i < scene.getFramebuffers().size(); i++)
    {
        commandBuffers->beginCommandBuffers(i);
            commandBuffers->beginRenderPass(i, scene.getRenderPass());

                commandBuffers->bindPipeline(i, scene.getPipeline());

                commandBuffers->bindVertexBuffer(i, vertexBuffers);
                commandBuffers->bindIndexBuffer(i, mesh.getIndexBuffer());
                commandBuffers->draw(i, 12);

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
    
    /* commandpool transfer */

    CommandPool transferCommandPool;

    CommandPool::CommandPoolCreateInfo commandPoolCreateInfo  = {};
    commandPoolCreateInfo.physicalDevice                      = &coreComponents.getPhysicalDevice();
    commandPoolCreateInfo.logicalDevice                       = &coreComponents.getLogicalDevice();
    commandPoolCreateInfo.queueFamilyIndex                    = coreComponents.getPhysicalDevice().getQueueFamilies().transferFamily.value();

    if (CommandPool::createCommandPool(&transferCommandPool, commandPoolCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandPool failed!");
    } else
    {
        Logger::printInfo("main::init", "createCommandPool!");
    }

    /* vertex buffer */

    std::vector<float> vertices =
    {
        -1.0f, +0.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
        +0.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
        +0.0f, +0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,

        +1.0f, +0.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
        -0.0f, +1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
        +1.0f, +1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
        +0.0f, +0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
    };

    std::vector<uint16_t> indices =
    {
        0, 1, 2,
        0, 3, 1,

        4, 5, 6,
        4, 7, 5,
    };

    Mesh::MeshCreateInfo meshCreateInfo = {};
    meshCreateInfo.physicalDevice       = &coreComponents.getPhysicalDevice();
    meshCreateInfo.logicalDevice        = &coreComponents.getLogicalDevice();
    meshCreateInfo.commandPool          = &transferCommandPool;
    meshCreateInfo.vertices             = vertices;
    meshCreateInfo.indices              = indices;

    if (Mesh::createMesh(&mesh, meshCreateInfo) != 0)
    {
        Logger::printError("main::init", "createMesh failed!");
    } else
    {
        Logger::printSuccess("main::init", "createMesh succeed!");
    }

    transferCommandPool.clean();

    /*vertex binding */

    VkVertexInputBindingDescription vertexBindingDescription  = {};
    vertexBindingDescription.binding                          = 0;
    vertexBindingDescription.stride                           = 7 * sizeof(float);
    vertexBindingDescription.inputRate                        = VK_VERTEX_INPUT_RATE_VERTEX;

    /* attributs description */

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    attributeDescriptions.resize(2);

    attributeDescriptions[0].binding                                          = 0;
    attributeDescriptions[0].location                                         = 0;
    attributeDescriptions[0].format                                           = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset                                           = 0.0f;     //offset of vertex positions

    attributeDescriptions[1].binding                                          = 0;
    attributeDescriptions[1].location                                         = 1;
    attributeDescriptions[1].format                                           = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset                                           = 3 * sizeof(float);     //offset of vertex positions

    /* shaders */

    Shader::ShaderCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.vertexShaderCode         = File::readFile("res/shaders/spir-v/main.vert");
    shaderCreateInfo.fragmentShaderCode       = File::readFile("res/shaders/spir-v/main.frag");

    /* renderpass */

    RenderPass::RenderPassAttachmentsInfo renderPassAttachmentsInfo = {};
    renderPassAttachmentsInfo.format                                = coreComponents.getSwapChain().getImageFormat();
    renderPassAttachmentsInfo.samples                               = VK_SAMPLE_COUNT_1_BIT;
    renderPassAttachmentsInfo.loadOp                                = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassAttachmentsInfo.storeOp                               = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassAttachmentsInfo.stencilLoadOp                         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassAttachmentsInfo.stencilStoreOp                        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassAttachmentsInfo.initialLayout                         = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassAttachmentsInfo.finalLayout                           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    /* pipeline */

    Pipeline::RasterizationInfo rasterizationInfo = {};
    rasterizationInfo.polygonMode                 = VK_POLYGON_MODE_FILL;
    rasterizationInfo.cullMode                    = VK_CULL_MODE_FRONT_BIT;
    rasterizationInfo.frontFace                   = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.lineWidth                   = 1.0f;

    Pipeline::VertexInputInfo vertexInputInfo   = {};
    vertexInputInfo.vertexBindingDescription    = vertexBindingDescription;
    vertexInputInfo.vertexAttributeDescriptions = attributeDescriptions;

    /* scene */

    Scene::SceneCreateInfo sceneCreateInfo      = {};
    sceneCreateInfo.coreComponents              = &coreComponents;
    sceneCreateInfo.shaderCreateInfo            = shaderCreateInfo;
    sceneCreateInfo.renderPassAttachmentsInfo   = renderPassAttachmentsInfo;
    sceneCreateInfo.rasterizationInfo           = rasterizationInfo;
    sceneCreateInfo.vertexInputInfo             = vertexInputInfo;
    sceneCreateInfo.drawFunction                = drawAll;

    if (Scene::createScene(&scene, sceneCreateInfo) != 0)
    {
        Logger::printError("main::init", "createScene failed!");
    } else
    {
        Logger::printSuccess("main::init", "createScene succeed!");
    }
}

void update()
{

}

void render()
{
    coreComponents.getWindow().update();

    scene.render();
}

void clean()
{
    Logger::init("___CLEAN__RESOURCES___");

    scene.clean();

    mesh.clean();

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

    coreComponents.getLogicalDevice().wait();

    clean();

    return 0;
}
