#include "MainRenderPass.h"
#include <scene/Scene.h>
#include <core/Engine.h>
#include <scene/mesh/MeshComponent.h>

MainRenderPass::MainRenderPass(const HashString& InName)
	: RenderPass( InName )
{
}

MainRenderPass::~MainRenderPass()
{
}

void MainRenderPass::InitPass()
{
	RendererPtr RendererInstance = Engine::GetRendererInstance();

	int Width = RendererInstance->GetWidth();
	int Height = RendererInstance->GetHeight();

	FrameBufferInstance->SetSize(Width, Height, false);
	FrameBufferInstance->SetAllowExternalDepthReset(false);
	FrameBufferInstance->SetDepthTexture(RendererInstance->GetRenderPass(std::string("ZPrepass"))->GetFrameBuffer()->GetDepthTexture());
	FrameBufferInstance->GenerateBuffer(1, true, true, false);
}

void MainRenderPass::DrawPass()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	FrameBufferInstance->Use();
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, FrameBufferInstance->GetWidth(), FrameBufferInstance->GetHeight());

	ScenePtr Scene = Engine::GetInstance()->GetScene();

	MainCamera = Scene->GetSceneComponent<CameraComponent>();

	View = MainCamera->CalculateViewMatrix();
	Proj = MainCamera->CalculateProjectionMatrix();

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		Model = MeshComp->GetParent()->Transform.GetMatrix();

		MaterialPtr Material = MeshComp->Material;
		Material->Use();
		SetupShader(Material->ShaderInstance);

		MeshComp->MeshData->Draw();
	}

	FrameBuffer::Unbind();
}

void MainRenderPass::OnResolutionChaged(int InWidth, int InHeight)
{
	FrameBufferInstance->SetSize(InWidth, InHeight, true);
}

void MainRenderPass::SetupShader(ShaderPtr InShader)
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


