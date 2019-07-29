#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader = CreateAndCompileShader(GL_VERTEX_SHADER, vShaderCode);
	unsigned int fragmentShader = CreateAndCompileShader(GL_FRAGMENT_SHADER, fShaderCode);

	ID = CreateAndLinkProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetUniformMatrix(const std::string &name, glm::mat4 & matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int Shader::CreateAndCompileShader(GLenum type, const char * shaderSourceCode)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSourceCode, NULL);
	glCompileShader(shader);

	int shaderCompileSuccess;
	char infoLog[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompileSuccess);
	if (!shaderCompileSuccess)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

unsigned int Shader::CreateAndLinkProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	char infoLog[1024];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	return program;
}
