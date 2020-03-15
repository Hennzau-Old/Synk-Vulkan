#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/CoreComponents.h"

#include "core/rendering/Shader.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"

#include "core/rendering/Framebuffer.h"

#include "core/rendering/CommandPool.h"
#include "core/rendering/CommandBuffers.h"

#include "core/rendering/WindowResizeManager.h"

#include "core/rendering/Submit.h"

class Scene
{
    public:

        /* static variables */



        /* structure */

        struct SceneCreateInfo
        {
            CoreComponents*       coreComponents    = nullptr;

            Shader::ShaderCreateInfo              shaderCreateInfo;
            RenderPass::RenderPassAttachmentsInfo renderPassAttachmentsInfo;
            Pipeline::RasterizationInfo           rasterizationInfo;
            Pipeline::VertexInputInfo             vertexInputInfo;
            Pipeline::DescriptorSetLayoutInfo     descriptorSetLayoutInfo;
            WindowResizeManager::FnPtr            drawFunction;
        };

        /* functions */

        Scene();
        ~Scene();

        void                      clean();
        void                      setData(const SceneCreateInfo& createInfo);

        void                      render();

        static int                createScene(Scene* scene, const SceneCreateInfo& createInfo);

        inline Shader&            getShader     () { return m_shader;     }
        inline RenderPass&        getRenderPass () { return m_renderPass; }
        inline Pipeline&          getPipeline   () { return m_pipeline;   }

        inline CommandPool&       getCommandPool    () { return m_commandPool;    }
        inline CommandBuffers&    getCommandBuffers () { return m_commandBuffers; }
        inline Submit&            getSubmit         () { return m_submit;         }

        SceneCreateInfo&          getInfo();

        inline WindowResizeManager&       getWindowResizeManager() { return m_windowResizeManager;  }
        inline std::vector<Framebuffer>&  getFramebuffers       () { return m_framebuffers;         }

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createShaders();
        int                       createRenderPass();
        int                       createPipeline();
        int                       createFramebuffers();
        int                       createCommandPool();
        int                       createCommandBuffers();
        int                       createWindowResizeManager();
        int                       createSubmit();
        int                       endCreation();

        /* variables */

        Shader                    m_shader;
        RenderPass                m_renderPass;
        Pipeline                  m_pipeline;

        std::vector<Framebuffer>  m_framebuffers;

        CommandPool               m_commandPool;
        CommandBuffers            m_commandBuffers;

        WindowResizeManager       m_windowResizeManager;
        Submit                    m_submit;

        SceneCreateInfo           m_info;
};
