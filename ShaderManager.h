#ifndef SHADER_MANAGER_HEADER
#define SHADER_MANAGER_HEADER

#include <unordered_map>
#include "Shader.h"
class ShaderManager
{
public:
	ShaderManager() {}

	Shader& addShader(const std::string& shaderName, const char* shaderVertex, const char* shaderFrag);

	Shader& getShader(const std::string& shaderName);

private:
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};
#endif