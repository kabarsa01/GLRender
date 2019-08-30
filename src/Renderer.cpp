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

#include "scene/SceneObjectBase.h"
#include "scene/mesh/MeshObject.h"
#include "scene/camera/CameraObject.h"

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
	glEnable(GL_DEPTH_TEST);

	CameraObj = ObjectBase::NewObject<CameraObject>();
	CameraObj->Transform.SetLocation(glm::vec3(0.0f, 0.0f, 3.0f));

	// output simple stats
	int maxVertexAttrib = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib);
	std::cout << "Maximum number of vertex attributes : " << maxVertexAttrib << std::endl;
	// shaders init
	DefaultShader = ObjectBase::NewObject<Shader, const GLchar *, const GLchar *>("./src/shaders/src/BasicVertexShader.vs", "./src/shaders/src/BasicFragmentShader.fs");
	DefaultAlbedo = ObjectBase::NewObject<Texture, const char *, bool>("./content/textures/container.jpg", false);
	SecondaryAlbedo = ObjectBase::NewObject<Texture, const char *, bool>("./content/textures/awesomeface.png", true);
	// mesh object init
	MeshObj = ObjectBase::NewObject<MeshObject>();
	MeshObj->Transform.SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
	MeshObj->Transform.SetLocation(glm::vec3(0.0f, 0.1f, 0.0f));

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

	std::vector<float>& MeshData = MeshObj->GetMeshComponent()->GetVerticesData();
	std::vector<unsigned int>& IndicesData = MeshObj->GetMeshComponent()->GetIndicesData();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, MeshData.size() * sizeof(float), MeshData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesData.size() * sizeof(unsigned int), IndicesData.data(), GL_STATIC_DRAW);

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
	glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// prerender init
	MeshObj->Transform.SetRotation(glm::vec3(-60.0f, 0.0f, 10.0f * (float)glfwGetTime()));
	glm::mat4 View = CameraObj->GetCameraComponent()->CalculateViewMatrix();
	glm::mat4 Proj = CameraObj->GetCameraComponent()->CalculateProjectionMatrix();

	// use default shader
	DefaultShader->Use();
	DefaultShader->SetUniformMatrix("model", MeshObj->Transform.GetMatrix());
	DefaultShader->SetUniformMatrix("view", View);
	DefaultShader->SetUniformMatrix("projection", Proj);

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


