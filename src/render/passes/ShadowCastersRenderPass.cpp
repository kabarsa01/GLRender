#include "./ShadowCastersRenderPass.h"
#include <scene/Scene.h>
#include <core/Engine.h>
#include <data/DataManager.h>
#include <scene/mesh/MeshComponent.h>
#include <scene/light/LightObject.h>

#include <glm/glm.hpp>

ShadowCastersRenderPass::ShadowCastersRenderPass(const HashString& InName)
	: RenderPass( InName )
{
}

ShadowCastersRenderPass::~ShadowCastersRenderPass()
{
}

void ShadowCastersRenderPass::InitPass()
{
	RendererPtr RendererInstance = Engine::GetRendererInstance();

	int Width = RendererInstance->GetWidth();
	int Height = RendererInstance->GetHeight();

	FrameBufferInstance->SetSize(1024, 1024, false);
	FrameBufferInstance->GenerateBuffer(0, true, true, true);

	DataManager *DM = DataManager::GetInstance();
	std::string VertexPath = "./src/shaders/src/ZPrepassVertexShader.vs";
	std::string FragmentPath = "./src/shaders/src/ZPrepassFragmentShader.fs";
	std::string ShaderId = VertexPath + FragmentPath;
	DepthShader = DM->RequestResourceByType<Shader, const std::string&, const std::string&>(ShaderId, VertexPath, FragmentPath);
}

void ShadowCastersRenderPass::DrawPass()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	FrameBufferInstance->Use();
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, 1024, 1024);

	ScenePtr Scene = Engine::GetInstance()->GetScene();
	MainLight = Scene->GetSceneComponent<LightComponent>();

	View = MainLight->GetParent()->Transform.CalculateViewMatrix();//CalculateViewMatrix();
	float near_plane = 1.0f, far_plane = 100.0f;
	Proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);

	// go through mesh components and draw them using assigned materials
	DepthShader->Use();
	DepthShader->SetMat4("view", View);
	DepthShader->SetMat4("projection", Proj);

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		MeshComp->GetParent()->Transform.SetRotation({ 10.0f * (float)glfwGetTime(), 0.0f , -90.0f });
		Model = MeshComp->GetParent()->Transform.GetMatrix();
		DepthShader->SetMat4("model", Model);
		MeshComp->MeshData->Draw();
	}

	FrameBuffer::Unbind();
}

void ShadowCastersRenderPass::OnResolutionChaged(int InWidth, int InHeight)
{
//	FrameBufferInstance->SetSize(InWidth, InHeight, true);
}


