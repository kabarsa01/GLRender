#include "Renderer.h"

#include <shaders/Shader.h>
#include <resources/Texture.h>

#include <GLFW/glfw3.h>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Class.h"
#include "core/Engine.h"

#include "data/DataManager.h"

#include "scene/Scene.h"
#include "scene/SceneObjectBase.h"
#include "scene/mesh/MeshObject.h"
#include "scene/camera/CameraObject.h"

#include "import/MeshImporter.h"

#include "common/HashString.h"

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

void Renderer::Initialize()
{
	ObjectBase::Initialize();
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	MaterialPtr Mat = ObjectBase::NewObject<Material, HashString>(std::string("DefaultMaterial"));
	Mat->AlbedoMapPath = "./content/root/Aset_wood_root_M_rkswd_4K_Albedo.jpg";
	Mat->NormalMapPath = "./content/root/Aset_wood_root_M_rkswd_4K_Normal_LOD0.jpg";
	Mat->VertexShaderPath = "./src/shaders/src/BasicVertexShader.vs";
	Mat->FragmentShaderPath = "./src/shaders/src/BasicFragmentShader.fs";

	Mat->Load();
	Mat->InitializeBuffers();

	MeshImporter Importer;
	//Importer.Import("./content/nanosuit/nanosuit.obj");
	Importer.Import("./content/root/Aset_wood_root_M_rkswd_LOD0.FBX");
	for (unsigned int MeshIndex = 0; MeshIndex < Importer.GetMeshes().size(); MeshIndex++)
	{
		std::shared_ptr<MeshObject> MO = ObjectBase::NewObject<MeshObject>();
		MO->GetMeshComponent()->MeshData = Importer.GetMeshes()[MeshIndex];
		MO->GetMeshComponent()->Material = Mat;
		MO->GetMeshComponent()->MeshData->SetupBufferObjects();
		MO->Transform.SetLocation({ 0.0f, -7.0f, 0.0f });
	}

	CameraObj = ObjectBase::NewObject<CameraObject>();
	CameraObj->Transform.SetLocation(glm::vec3(0.0f, 15.0f, 30.0f));
	CameraObj->Transform.SetRotation(glm::vec3(30.0f, 0.0f, 0.0f));
	CameraObj->GetCameraComponent()->SetNearPlane(0.15f);
	CameraObj->GetCameraComponent()->SetFarPlane(1000.f);

	// output simple stats
	int maxVertexAttrib = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib);
	std::cout << "Maximum number of vertex attributes : " << maxVertexAttrib << std::endl;
}

void Renderer::RenderFrame()
{
//	glDepthMask(GL_FALSE);
//	glDepthFunc(GL_LESS);
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ScenePtr Scene = Engine::GetInstance()->GetScene();

	MainCamera = Scene->GetSceneComponent<CameraComponent>();

	View = MainCamera->CalculateViewMatrix();
	Proj = MainCamera->CalculateProjectionMatrix();

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		MeshComp->GetParent()->Transform.SetRotation({ 10.0f * (float)glfwGetTime(), 0.0f , -90.0f });
		Model = MeshComp->GetParent()->Transform.GetMatrix();

		MaterialPtr Material = MeshComp->Material;
		Material->Use();
		SetupShader(Material->ShaderInstance);

		MeshComp->MeshData->Draw();
	}
}

void Renderer::SetupShader(ShaderPtr InShader)
{
	InShader->SetMat4("model", Model);
	InShader->SetMat4("view", View);
	InShader->SetMat4("projection", Proj);

	// uniforms setup once
	InShader->SetVec3("ambient_color", { 0.04f, 0.04f, 0.045f });
	InShader->SetVec3("light_dir", { -1.0f, -0.5f, -0.5f });
	InShader->SetVec3("light_color", { 0.95f, 0.95f, 0.95f });
	InShader->SetVec3("spec_color", { 1.0f, 0.0f, 0.0f });
	InShader->SetFloat("spec_strength", 0.0f);
	InShader->SetVec3("view_pos", MainCamera->GetParent()->Transform.GetLocation());
	InShader->SetInt("albedo", 0);
	InShader->SetInt("normalMap", 1);
}




