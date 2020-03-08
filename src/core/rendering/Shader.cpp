#include "core/rendering/Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::clean()
{
    vkDestroyShaderModule(m_info.logicalDevice->getLogicalDevice(), m_vertexShader, nullptr);
    vkDestroyShaderModule(m_info.logicalDevice->getLogicalDevice(), m_fragmentShader, nullptr);
}

void Shader::setData(const ShaderCreateInfo& createInfo)
{
    m_info = createInfo;
}

int Shader::createShader()
{
    std::vector<char> vertexShaderCode  (m_info.vertexShaderCode.begin(), m_info.vertexShaderCode.end());
    std::vector<char> fragmentShaderCode(m_info.fragmentShaderCode.begin(), m_info.fragmentShaderCode.end());

    m_vertexShader    = createShaderModule(vertexShaderCode);
    m_fragmentShader  = createShaderModule(fragmentShaderCode);

    if (m_vertexShader == VK_NULL_HANDLE)
    {
        Logger::printError("Shader::createShader", "createVertexShaderModule failed!");
    }

    if (m_fragmentShader == VK_NULL_HANDLE)
    {
        Logger::printError("Shader::createShader", "createFragmentShaderModule failed!");
    }

    return 0;
}

int Shader::createShaderStage()
{
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
    vertexShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module                          = m_vertexShader;
    vertexShaderStageInfo.pName                           = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
    fragmentShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module                          = m_fragmentShader;
    fragmentShaderStageInfo.pName                           = "main";

    m_shaderStages.push_back(vertexShaderStageInfo);
    m_shaderStages.push_back(fragmentShaderStageInfo);

    return 0;
}

VkShaderModule Shader::createShaderModule(const std::vector<char>& code)
{
    VkShaderModule shaderModule;

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize                 = code.size();
    createInfo.pCode                    = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(m_info.logicalDevice->getLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        Logger::printError("Shader::createShaderModule", "vkCreateShaderModule failed!");

        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

int Shader::createShader(Shader* shader, const ShaderCreateInfo& createInfo)
{
    shader->setData(createInfo);

    return shader->createShader() + shader->createShaderStage();
}

Shader::ShaderCreateInfo& Shader::getInfo()
{
  return m_info;
}
