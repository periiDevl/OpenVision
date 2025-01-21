#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H
#include "glad/glad.h"
#include<iostream>
#include <fstream>  // For std::ifstream
#include <sstream>  // For std::stringstream
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile) 
	{


		std::string vertexCode = vertexFile;
		std::string fragmentCode = fragmentFile;


		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		compileErrors(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		compileErrors(fragmentShader, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		compileErrors(ID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void activate()
	{
		glUseProgram(ID);
	}

	void dispose()
	{
		glDeleteProgram(ID);
	}

	// Scalar types
	void setInt(const std::string& element, int value) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), value);
	}

	void setFloat(const std::string& element, float value) {
		glUniform1f(glGetUniformLocation(ID, element.c_str()), value);
	}

	void setBool(const std::string& element, bool value) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), value ? 1 : 0);
	}

	// Vector types
	void setVec2(const std::string& element, const glm::vec2& value) {
		glUniform2fv(glGetUniformLocation(ID, element.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& element, const glm::vec3& value) {
		glUniform3fv(glGetUniformLocation(ID, element.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string& element, const glm::vec4& value) {
		glUniform4fv(glGetUniformLocation(ID, element.c_str()), 1, &value[0]);
	}

	// Matrix types
	void setMat2(const std::string& element, const glm::mat2& value) {
		glUniformMatrix2fv(glGetUniformLocation(ID, element.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void setMat3(const std::string& element, const glm::mat3& value) {
		glUniformMatrix3fv(glGetUniformLocation(ID, element.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void setMat4(const std::string& element, const glm::mat4& value) {
		glUniformMatrix4fv(glGetUniformLocation(ID, element.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	// Array types
	void setIntArray(const std::string& element, const std::vector<int>& values) {
		glUniform1iv(glGetUniformLocation(ID, element.c_str()), values.size(), values.data());
	}

	void setFloatArray(const std::string& element, const std::vector<float>& values) {
		glUniform1fv(glGetUniformLocation(ID, element.c_str()), values.size(), values.data());
	}

	void setVec2Array(const std::string& element, const std::vector<glm::vec2>& values) {
		glUniform2fv(glGetUniformLocation(ID, element.c_str()), values.size(), &values[0][0]);
	}

	void setVec3Array(const std::string& element, const std::vector<glm::vec3>& values) {
		glUniform3fv(glGetUniformLocation(ID, element.c_str()), values.size(), &values[0][0]);
	}

	void setVec4Array(const std::string& element, const std::vector<glm::vec4>& values) {
		glUniform4fv(glGetUniformLocation(ID, element.c_str()), values.size(), &values[0][0]);
	}

	void setMat4Array(const std::string& element, const std::vector<glm::mat4>& values) {
		glUniformMatrix4fv(glGetUniformLocation(ID, element.c_str()), values.size(), GL_FALSE, &values[0][0][0]);
	}

	// Texture samplers
	void setSampler1D(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

	void setSampler2D(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

	void setSampler3D(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

	void setSamplerCube(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

	void setSampler2DArray(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

	void setSamplerCubeArray(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

	// Uniform Block (array of uniforms)
	void setUniformBlock(const std::string& blockName, GLuint bindingPoint) {
		GLuint blockIndex = glGetUniformBlockIndex(ID, blockName.c_str());
		glUniformBlockBinding(ID, blockIndex, bindingPoint);
	}

	// Generic Sampler (for array textures, etc.)
	void setSampler2DArrayGeneric(const std::string& element, GLuint textureUnit) {
		glUniform1i(glGetUniformLocation(ID, element.c_str()), textureUnit);
	}

private:
	void compileErrors(unsigned int shader, const char* type)
	{
		GLint hasCompiled;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
}
};

#endif 
