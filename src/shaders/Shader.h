#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "data/Resource.h"
#include "core/ObjectBase.h"

class Shader : public Resource
{
public:
	// shader program ID
	unsigned int ID;
	
	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	virtual ~Shader();

	void Use();
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetUniformMatrix(const std::string &name, glm::mat4 & matrix) const;
protected:
	unsigned int CreateAndCompileShader(GLenum type, const char* shaderSourceCode);
	unsigned int CreateAndLinkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};

