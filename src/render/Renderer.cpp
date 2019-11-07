#include "Renderer.h"

#include <shaders/Shader.h>
#include <resources/Texture.h>
#include <resources/Texture2D.h>

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
#include "scene/light/LightObject.h"

#include "import/MeshImporter.h"

#include "common/HashString.h"

#include "passes/MainRenderPass.h"
#include "passes/ZPrepassRenderPass.h"
#include "passes/ScreenOutputRenderPass.h"
#include "passes/ShadowCastersRenderPass.h"

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

//std::vector<Vertex> QuadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//		// positions          // texCoords
//	{{-1.0f,  1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
//	{{-1.0f, -1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
//	{{ 1.0f, -1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
//	{{ 1.0f,  1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}
//};
//
//std::vector<unsigned int> QuadIndices = {
//	0, 1, 2,
//	0, 2, 3
//};

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

void Renderer::OnInitialize()
{
	ObjectBase::OnInitialize();
}

void Renderer::Init()
{
	DataManager* DM = DataManager::GetInstance();

	LightObjectPtr LightObj = ObjectBase::NewObject<LightObject>();
	LightObj->Transform.SetRotation(glm::vec3(45.0f, -90.0f, 0.0f));
	LightObj->Transform.SetLocation(LightObj->Transform.GetForwardVector() * -50.0f);
	glm::mat4 LightSpaceMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f) * LightObj->Transform.CalculateViewMatrix();

	CameraObjectPtr CameraObj = ObjectBase::NewObject<CameraObject>();
	CameraObj->Transform.SetLocation(glm::vec3(0.0f, 15.0f, 30.0f));
	CameraObj->Transform.SetRotation(glm::vec3(30.0f, 180.0f, 0.0f));
	//CameraObj->Transform.SetRotation(glm::vec3(45.0f, -90.0f, 0.0f));
	//CameraObj->Transform.SetLocation(glm::vec3(0.0f, 5.0f, 0.0f) + CameraObj->Transform.GetForwardVector() * -35.0f);
	CameraObj->GetCameraComponent()->SetNearPlane(0.15f);
	CameraObj->GetCameraComponent()->SetFarPlane(100.f);
	//CameraObj->GetCameraComponent()->SetFOV(60.f);
	//-----------------------------------------------------------------------------------------------
	// setup default material
	MaterialPtr Mat = ObjectBase::NewObject<Material, HashString>(std::string("DefaultMaterial"));
	Mat->SetShaderPath("./src/shaders/src/BasicVertexShader.vs", "./src/shaders/src/BasicFragmentShader.fs");
	std::string AlbedoPath("./content/gun/Textures/Cerberus_A.tga");//"./content/root/Aset_wood_root_M_rkswd_4K_Albedo.jpg");
	std::string NormalPath("./content/gun/Textures/Cerberus_N.tga");//"./content/root/Aset_wood_root_M_rkswd_4K_Normal_LOD0.jpg");
	std::string MetallnessPath("./content/gun/Textures/Cerberus_M.tga");//"./content/root/Aset_wood_root_M_rkswd_4K_Normal_LOD0.jpg");
	std::string RoughnessPath("./content/gun/Textures/Cerberus_R.tga");//"./content/root/Aset_wood_root_M_rkswd_4K_Roughness.jpg");
	std::string AOPath("./content/gun/Textures/Raw/Cerberus_AO.tga");//"./content/root/Aset_wood_root_M_rkswd_4K_Cavity.jpg");
	Texture2DPtr AlbedoMap = DM->RequestResourceByType<Texture2D, const std::string&, bool, bool, bool>(AlbedoPath, AlbedoPath, false, true, false);
	Texture2DPtr NormalMap = DM->RequestResourceByType<Texture2D, const std::string&, bool, bool, bool>(NormalPath, NormalPath, false, true, true);
	Texture2DPtr MetallnessMap = DM->RequestResourceByType<Texture2D, const std::string&, bool, bool, bool>(MetallnessPath, MetallnessPath, false, true, false);
	Texture2DPtr RoughnessMap = DM->RequestResourceByType<Texture2D, const std::string&, bool, bool, bool>(RoughnessPath, RoughnessPath, false, true, false);
	Texture2DPtr AOMap = DM->RequestResourceByType<Texture2D, const std::string&, bool, bool, bool>(AOPath, AOPath, false, true, false);
	Mat->AddTextureParam("AlbedoMap", AlbedoPath, AlbedoMap, 0);
	Mat->AddTextureParam("NormalMap", NormalPath, NormalMap, 1);
	Mat->AddTextureParam("MetallnessMap", MetallnessPath, MetallnessMap, 2);
	Mat->AddTextureParam("RoughnessMap", RoughnessPath, RoughnessMap, 3);
	Mat->AddTextureParam("AOMap", AOPath, AOMap, 4);
	Mat->AddTextureParam("ShadowMap", "", nullptr, 5);
	// uniforms setup once
	Mat->AddUniformParam<glm::vec3>("ambient_color", { 0.04f, 0.04f, 0.045f });
	Mat->AddUniformParam<glm::vec3>("light_dir", LightObj->Transform.GetForwardVector());//{ -1.0f, -0.5f, -0.5f });
	Mat->AddUniformParam<glm::mat4>("light_view", LightSpaceMatrix);
	Mat->AddUniformParam<glm::vec3>("light_color", { 6.0f, 6.0f, 6.0f });
	Mat->AddUniformParam<glm::vec3>("spec_color", { 1.0f, 0.0f, 0.0f });
	Mat->AddUniformParam<float>("spec_strength", 0.0f);

	Mat->Load();
	Mat->InitializeBuffers();
	Mat->SetupParams();

	{
		MeshImporter Importer;
		//Importer.Import("./content/nanosuit/nanosuit.obj");
		//Importer.Import("./content/root/Aset_wood_root_M_rkswd_LOD0.FBX");
		Importer.Import("./content/gun/Cerberus_LP.FBX");
		for (unsigned int MeshIndex = 0; MeshIndex < Importer.GetMeshes().size(); MeshIndex++)
		{
			MeshObjectPtr MO = ObjectBase::NewObject<MeshObject>();
			MO->GetMeshComponent()->MeshData = Importer.GetMeshes()[MeshIndex];
			MO->GetMeshComponent()->Material = Mat;
			MO->GetMeshComponent()->MeshData->SetupBufferObjects();
			MO->Transform.SetLocation({ 0.0f, -1.0f, -3.0f });
			MO->Transform.SetScale({ 0.3f, 0.3f, 0.3f });
		}
	}
	{
		MeshImporter Importer;
		//Importer.Import("./content/nanosuit/nanosuit.obj");
		//Importer.Import("./content/root/Aset_wood_root_M_rkswd_LOD0.FBX");
		Importer.Import("./content/gun/Cerberus_LP.FBX");
		for (unsigned int MeshIndex = 0; MeshIndex < Importer.GetMeshes().size(); MeshIndex++)
		{
			MeshObjectPtr MO = ObjectBase::NewObject<MeshObject>();
			MO->GetMeshComponent()->MeshData = Importer.GetMeshes()[MeshIndex];
			MO->GetMeshComponent()->Material = Mat;
			MO->GetMeshComponent()->MeshData->SetupBufferObjects();
			MO->Transform.SetLocation({ -25.0f, -12.0f, -13.0f });
			MO->Transform.SetScale({ 0.3f, 0.3f, 0.3f });
		}
	}

	// output simple stats
	int maxVertexAttrib = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib);
	std::cout << "Maximum number of vertex attributes : " << maxVertexAttrib << std::endl;

	// instantiate and register render passes
	ZPrepassRenderPassPtr ZPrepass = ObjectBase::NewObject<ZPrepassRenderPass, const HashString&>(std::string("ZPrepass"));
	ZPrepass->InitPass();
	RegisterRenderPass(ZPrepass);
	ShadowCastersRenderPassPtr ShadowCastersPass = ObjectBase::NewObject<ShadowCastersRenderPass, const HashString&>(std::string("ShadowCasters"));
	ShadowCastersPass->InitPass();
	RegisterRenderPass(ShadowCastersPass);
	MainRenderPassPtr MainPass = ObjectBase::NewObject<MainRenderPass, const HashString&>(std::string("MainPass"));
	MainPass->InitPass();
	RegisterRenderPass(MainPass);
	ScreenOutputRenderPassPtr ScreenOutputPass = ObjectBase::NewObject<ScreenOutputRenderPass, const HashString&>(std::string("ScreenOutput"));
	ScreenOutputPass->InitPass();
	RegisterRenderPass(ScreenOutputPass);
}

void Renderer::RenderFrame()
{
	for (unsigned int PassIndex = 0; PassIndex < RenderPasses.size(); PassIndex++)
	{
		RenderPasses[PassIndex]->DrawPass();
	}
}

void Renderer::SetResolution(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;

	for (unsigned int PassIndex = 0; PassIndex < RenderPasses.size(); PassIndex++)
	{
		RenderPasses[PassIndex]->OnResolutionChaged(InWidth, InHeight);
	}
}

int Renderer::GetWidth() const
{
	return Width;
}

int Renderer::GetHeight() const
{
	return Height;
}

RenderPassPtr Renderer::GetRenderPass(const std::string& InName)
{
	return GetRenderPass(HashString(InName));
}

RenderPassPtr Renderer::GetRenderPass(const HashString& InName)
{
	if (RenderPassMap.find(InName) != RenderPassMap.end())
	{
		return RenderPasses[ RenderPassMap[InName] ];
	}

	return nullptr;
}

void Renderer::RegisterRenderPass(RenderPassPtr InRenderPass)
{
	if (RenderPassMap.find(InRenderPass->GetName()) != RenderPassMap.end())
	{
		return;
	}

	RenderPasses.push_back(InRenderPass);
	RenderPassMap[InRenderPass->GetName()] = RenderPasses.size() - 1;
}



