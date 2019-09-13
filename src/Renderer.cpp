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

#include "import/MeshImporter.h"

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

	vector<std::string> Paths{
		"./content/root/Aset_wood_root_M_rkswd_4K_Albedo.jpg",
	};

	MeshImporter Importer;
	//Importer.Import("./content/nanosuit/nanosuit.obj");
	Importer.Import("./content/root/Aset_wood_root_M_rkswd_LOD0.FBX");
	for (unsigned int MeshIndex = 0; MeshIndex < Importer.GetMeshes().size(); MeshIndex++)
	{
		std::shared_ptr<MeshObject> MO = ObjectBase::NewObject<MeshObject>();
		MO->GetMeshComponent()->MeshDataPtr = Importer.GetMeshes()[MeshIndex];
		MeshObjects.push_back(MO);
		MO->Transform.SetLocation({ 0.0f, -7.0f, 0.0f });

		unsigned int TextureIndex = MeshIndex < Paths.size() ? MeshIndex : Paths.size() - 1;
		Albedos.push_back(ObjectBase::NewObject<Texture, const char *, bool>(Paths[TextureIndex].c_str(), false));
	}

	CameraObj = ObjectBase::NewObject<CameraObject>();
	CameraObj->Transform.SetLocation(glm::vec3(0.0f, 15.0f, 30.0f));
	CameraObj->Transform.SetRotation(glm::vec3(30.0f, 0.0f, 0.0f));

	// output simple stats
	int maxVertexAttrib = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib);
	std::cout << "Maximum number of vertex attributes : " << maxVertexAttrib << std::endl;
	// shaders init
	DefaultShader = ObjectBase::NewObject<Shader, const GLchar *, const GLchar *>("./src/shaders/src/BasicVertexShader.vs", "./src/shaders/src/BasicFragmentShader.fs");
	DefaultAlbedo = ObjectBase::NewObject<Texture, const char *, bool>("./content/textures/container.jpg", false);
	SecondaryAlbedo = ObjectBase::NewObject<Texture, const char *, bool>("./content/textures/awesomeface.png", true);

	// use default shader
	DefaultShader->Use();
	// uniforms setup once
	DefaultShader->SetVec3("ambient_color", { 0.2f, 0.2f, 0.25f });
	DefaultShader->SetVec3("light_dir",{ -1.0f, -0.5f, -0.5f });
	DefaultShader->SetVec3("light_color", { 1.0f, 1.0f, 0.9f });
	DefaultShader->SetVec3("spec_color", { 1.0f, 0.0f, 0.0f });
	DefaultShader->SetFloat("spec_strength", 0.5f);
	DefaultShader->SetVec3("view_pos", CameraObj->Transform.GetLocation());
	DefaultShader->SetInt("albedo", 0);
	DefaultShader->SetInt("secondaryAlbedo", 1);

	for (unsigned int MeshIndex = 0; MeshIndex < MeshObjects.size(); MeshIndex++)
	{
		MeshObjects[MeshIndex]->GetMeshComponent()->MeshDataPtr->SetupBufferObjects();
	}
}

void Renderer::RenderFrame()
{
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// prerender init
	//MeshObj->Transform.SetRotation(glm::vec3(-60.0f, 0.0f, 10.0f * (float)glfwGetTime()));
	glm::mat4 View = CameraObj->GetCameraComponent()->CalculateViewMatrix();
	glm::mat4 Proj = CameraObj->GetCameraComponent()->CalculateProjectionMatrix();

	// use default shader
	DefaultShader->Use();

	// uniforms and variables
//	defaultShader->SetInt("albedo", 0);
//	defaultShader->SetInt("secondaryAlbedo", 1);

	//DefaultAlbedo->Use(GL_TEXTURE0);
	SecondaryAlbedo->Use(GL_TEXTURE1);

	//MeshObj->GetMeshComponent()->MeshDataPtr->Draw();
	for (unsigned int MeshIndex = 0; MeshIndex < MeshObjects.size(); MeshIndex++)
	{
		Albedos[MeshIndex]->Use(GL_TEXTURE0);

		DefaultShader->SetMat4("model", MeshObjects[MeshIndex]->Transform.GetMatrix());
		DefaultShader->SetMat4("view", View);
		DefaultShader->SetMat4("projection", Proj);

		MeshObjects[MeshIndex]->Transform.SetRotation({10.0f * (float)glfwGetTime(), 0.0f , -90.0f});
		MeshObjects[MeshIndex]->GetMeshComponent()->MeshDataPtr->Draw();
	}
}


