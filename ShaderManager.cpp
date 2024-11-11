#include "ShaderManager.h"

Shader& ShaderManager::addShader(const std::string& shaderName, const char* shaderVertex, const char* shaderFrag)
{
	shaders[shaderName] = std::make_unique<Shader>(shaderVertex, shaderFrag);
	return getShader(shaderName);
}

Shader& ShaderManager::getShader(const std::string& shaderName)
{
	return *shaders[shaderName];
}
