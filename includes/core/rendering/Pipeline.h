#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/SwapChain.h"
#include "core/rendering/Shader.h"
#include "core/rendering/RenderPass.h"

class Pipeline
{
    public:

        /* static variables */



        /* structure */

        struct RasterizationInfo
        {
            VkPolygonMode   polygonMode;
            VkCullModeFlags cullMode;
            VkFrontFace     frontFace;

            float           lineWidth;
        };

        struct VertexInputInfo
        {
            VkVertexInputBindingDescription                 vertexBindingDescription;
            std::vector<VkVertexInputAttributeDescription>  vertexAttributeDescriptions;
        };

        struct DescriptorSetLayoutInfo
        {
            std::vector<VkDescriptorSetLayoutBinding>       descriptors;
        };

        struct PipelineCreateInfo
        {
            LogicalDevice*          logicalDevice = nullptr;
            SwapChain*              swapChain     = nullptr;
            Shader*                 shader        = nullptr;
            RenderPass*             renderPass    = nullptr;

            RasterizationInfo       rasterizationInfo;
            VertexInputInfo         vertexInputInfo;
            DescriptorSetLayoutInfo descriptorsInfo;
        };

        /* functions */

        Pipeline();
        ~Pipeline();

        void                      clean();
        void                      setData(const PipelineCreateInfo& createInfo);

        static int                createPipeline(Pipeline* pipeline, const PipelineCreateInfo& createInfo);

        inline VkPipeline&            getPipeline           () { return m_pipeline; }
        inline VkPipelineLayout&      getPipelineLayout     () { return m_pipelineLayout; }
        inline VkDescriptorSetLayout& getDescriptorSetLayout() {return m_descriptorSetLayout; }

        PipelineCreateInfo&       getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                         createPipeline();

        /* variables */

        VkPipeline                  m_pipeline;
        VkPipelineLayout            m_pipelineLayout;
        VkDescriptorSetLayout       m_descriptorSetLayout;

        PipelineCreateInfo          m_info;
};
