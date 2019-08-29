#include "Renderer.h"

#include <shaders/Shader.h>
#include <resources/Texture.h>

#include <GLFW/glfw3.h>
#include <cstdio>
#include <stdio.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <scene/SceneObjectBase.h>
#include <scene/camera/CameraObject.h>
#include <scene/mesh/MeshObject.h>

//======================================================================
const char* vertexShaderSource = "#version 330 core \n \
layout(location = 0) in vec3 aPos; \n \
layout(location = 1) in vec3 aColor; \n \
\n \
out vec3 vertexColor; \n \
\n \
void main() \n \
{ \n \
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n \
	vertexColor = aColor; \n \
}";
//======================================================================
const char* fragmentShaderSource = "#version 330 core \n \
uniform vec4 externalColor; \
\n \
in vec3 vertexColor; \n \
\n \
out vec4 FragColor; \n \
 \n \
void main() \n \
{ \n \
	FragColor = vec4(vertexColor, 1.0f) * externalColor; \n \
}";

//======================================================================
//======================================================================

//float vertices[] = {
//	// coords              // color             // uv coords
//	 0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // top right
//	 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // bottom right
//	-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // bottom left
//	-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // top left 
//};
//
//unsigned int indices[] = {  // note that we start from 0!
//	0, 1, 3,   // first triangle
//	1, 2, 3    // second triangle
//};

//======================================================================
//======================================================================

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}


void Renderer::Init()
{
	// output simple stats
	int maxVertexAttrib = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib);
	std::cout << "Maximum number of vertex attributes : " << maxVertexAttrib << std::endl;
	// shaders init
	DefaultShader = std::shared_ptr<Shader>(new Shader("./src/shaders/src/BasicVertexShader.vs", "./src/shaders/src/BasicFragmentShader.fs"));

	DefaultAlbedo = std::shared_ptr<Texture>(new Texture("./content/textures/container.jpg", false));
	SecondaryAlbedo = std::shared_ptr<Texture>(new Texture("./content/textures/awesomeface.png", true));

	MeshObj = ObjectBase::NewObject<MeshObject>();

	// use default shader
	DefaultShader->Use();
	// uniforms setup once
	DefaultShader->SetInt("albedo", 0);
	DefaultShader->SetInt("secondaryAlbedo", 1);

	// VAO & VBO init
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// VAO bind
	glBindVertexArray(VAO);

	float* MeshData = MeshObj->GetMeshComponent()->GetVerticesData().data();
	unsigned int* IndicesData = MeshObj->GetMeshComponent()->GetIndicesData().data();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshData/*vertices*/), MeshData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesData), IndicesData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::RenderFrame()
{
	// prerender init
	MeshObj->Transform.SetLocation(glm::vec3(0.1f, 0.2f, 0.0f));
	MeshObj->Transform.SetRotation(glm::vec3(0.0f, 0.0f, (float)glfwGetTime()));
	MeshObj->Transform.SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, glm::vec3(0.1f, 0.2f, 0.0f));
	//transform = glm::rotate(transform, (float)glfwGetTime()/*glm::radians(45.0f)*/, glm::vec3(0.0f, 0.0f, 1.0f));
	//transform = glm::scale(transform, glm::vec3(0.7f, 0.7f, 0.7f));

	// use default shader
	DefaultShader->Use();
	DefaultShader->SetUniformMatrix("transform", MeshObj->Transform.GetMatrix());

	// uniforms and variables
//	defaultShader->SetInt("albedo", 0);
//	defaultShader->SetInt("secondaryAlbedo", 1);

	DefaultAlbedo->Use(GL_TEXTURE0);
	SecondaryAlbedo->Use(GL_TEXTURE1);

	// bind VAO and draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


