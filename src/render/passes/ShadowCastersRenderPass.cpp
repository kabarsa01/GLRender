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

	FrameBufferInstance->SetSize(Width, Height, false);
	FrameBufferInstance->SetColorBuffersCount(0);
	FrameBufferInstance->SetUseDepth(true);
	FrameBufferInstance->SetUseStencil(false);
	FrameBufferInstance->CreateTextures();
	FrameBufferInstance->GenerateBuffers();

	DataManager *DM = DataManager::GetInstance();
	std::string VertexPath = "./src/shaders/src/ZPrepassVertexShader.vs";
	std::string FragmentPath = "./src/shaders/src/ZPrepassFragmentShader.fs";
	std::string ShaderId = VertexPath + FragmentPath;
	DepthShader = DM->RequestResourceByType<Shader, const std::string&, const std::string&>(ShaderId, VertexPath, FragmentPath);
}

void ShadowCastersRenderPass::DrawPass()
{
	FrameBufferInstance->Use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glDepthMask(GL_TRUE);
	glStencilMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, Width, Height);

	ScenePtr Scene = Engine::GetInstance()->GetScene();
	MainLight = Scene->GetSceneComponent<LightComponent>();

	View = MainLight->GetParent()->Transform.CalculateViewMatrix();//CalculateViewMatrix();
	float near_plane = 0.1f, far_plane = 100.0f;
	Proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	// go through mesh components and draw them using assigned materials
	DepthShader->Use();
	DepthShader->SetMat4("view", View);
	DepthShader->SetMat4("projection", Proj);

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		Model = MeshComp->GetParent()->Transform.GetMatrix();
		DepthShader->SetMat4("model", Model);
		MeshComp->MeshData->Draw();
	}

	FrameBuffer::Unbind();
}

void ShadowCastersRenderPass::OnResolutionChaged(int InWidth, int InHeight)
{
	// we preserve shadow resolution across different screen resolutions
//	FrameBufferInstance->SetSize(InWidth, InHeight, true);
}

void ShadowCastersRenderPass::SetShadowResolution(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;
}


