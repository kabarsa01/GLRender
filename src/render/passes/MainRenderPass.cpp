#include "MainRenderPass.h"
#include <scene/Scene.h>
#include <core/Engine.h>
#include <scene/mesh/MeshComponent.h>

#include <glm/glm.hpp>

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
	FrameBufferInstance->SetColorBuffersCount(1);
	FrameBufferInstance->SetUseDepth(true);
	FrameBufferInstance->SetUseStencil(true);
	FrameBufferInstance->SetAllowDepthReset(false);
	FrameBufferInstance->SetDepthTexture(RendererInstance->GetRenderPass(std::string("ZPrepass"))->GetFrameBuffer()->GetDepthTexture());
	FrameBufferInstance->CreateTextures();
	FrameBufferInstance->GenerateBuffers();
}

void MainRenderPass::DrawPass()
{
	FrameBufferInstance->Use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glStencilMask(GL_FALSE);//GL_TRUE);
	glDepthMask(GL_FALSE);//GL_TRUE);
	glDepthFunc(GL_LEQUAL);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, FrameBufferInstance->GetWidth(), FrameBufferInstance->GetHeight());

	ScenePtr Scene = Engine::GetInstance()->GetScene();

	MainCamera = Scene->GetSceneComponent<CameraComponent>();

	View = MainCamera->CalculateViewMatrix();
	Proj = MainCamera->CalculateProjectionMatrix();

	RendererPtr Rend = Engine::GetRendererInstance();
	Texture2DPtr ShadowMask = Rend->GetRenderPass(std::string("ShadowCasters"))->GetFrameBuffer()->GetDepthTexture();

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		Model = MeshComp->GetParent()->Transform.GetMatrix();

		MaterialPtr Material = MeshComp->Material;
		Material->UpdateTextureParam("ShadowMap", ShadowMask, false);
		Material->Use();
		Material->SetUniformParam<glm::mat4>("model", Model);
		Material->SetUniformParam<glm::mat4>("view", View);
		Material->SetUniformParam<glm::mat4>("projection", Proj);
		Material->SetUniformParam<glm::vec3>("view_pos", MainCamera->GetParent()->Transform.GetLocation());

		MeshComp->MeshData->Draw();
	}

	FrameBuffer::Unbind();
}

void MainRenderPass::OnResolutionChaged(int InWidth, int InHeight)
{
	FrameBufferInstance->SetSize(InWidth, InHeight, true);
}


