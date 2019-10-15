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

	RendererPtr Rend = Engine::GetRendererInstance();
	TexturePtr ShadowMask = Rend->GetRenderPass(std::string("ShadowCasters"))->GetFrameBuffer()->GetDepthTexture();
	ShadowMask->Use(GL_TEXTURE2);

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		Model = MeshComp->GetParent()->Transform.GetMatrix();

		MaterialPtr Material = MeshComp->Material;
		Material->Use();
		Material->ShaderInstance->SetInt("ShadowMap", 2);
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


