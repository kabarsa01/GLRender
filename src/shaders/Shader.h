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
	unsigned int ID = { static_cast<unsigned int>(-1) };
	
	// constructor reads and builds the shader
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	virtual ~Shader();

	// Inherited via Resource
	virtual bool Load() override;
	virtual bool Unload() override;

	void Use();
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void SetVec2(const std::string &name, const glm::vec2 &value) const;
	void SetVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void SetVec4(const std::string &name, const glm::vec4 &value) const;
	void SetVec4(const std::string &name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void SetMat2(const std::string &name, const glm::mat2 &mat) const;
	// ------------------------------------------------------------------------
	void SetMat3(const std::string &name, const glm::mat3 &mat) const;
	// ------------------------------------------------------------------------
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
protected:
	std::string VertexPath;
	std::string FragmentPath;

	unsigned int CreateAndCompileShader(GLenum type, const char* shaderSourceCode);
	unsigned int CreateAndLinkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};

typedef std::shared_ptr<Shader> ShaderPtr;

