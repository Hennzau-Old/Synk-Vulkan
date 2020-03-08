#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"

class Shader
{
    public:

        /* static variables */



        /* structure */

        struct ShaderCreateInfo
        {
            LogicalDevice*        logicalDevice = nullptr;

            std::string           vertexShaderCode;
            std::string           fragmentShaderCode;
        };

        /* functions */

        Shader();
        ~Shader();

        void                      clean();
        void                      setData(const ShaderCreateInfo& createInfo);

        static int                createShader(Shader* shader, const ShaderCreateInfo& createInfo);

        ShaderCreateInfo&         getInfo();

        inline std::vector<VkPipelineShaderStageCreateInfo>& getShaderStage() { return m_shaderStages; }

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createShader();
        int                       createShaderStage();

        VkShaderModule            createShaderModule(const std::vector<char>& code);

        /* variables */

        VkShaderModule            m_vertexShader;
        VkShaderModule            m_fragmentShader;

        std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;

        ShaderCreateInfo          m_info;
};
