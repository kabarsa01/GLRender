#include "./ZPrepassRenderPass.h"
#include "../Renderer.h"
#include "data/DataManager.h"
#include <core/Engine.h>
#include <scene/mesh/MeshComponent.h>

ZPrepassRenderPass::ZPrepassRenderPass(const HashString& InName)
	: RenderPass(InName)
{

}

ZPrepassRenderPass::~ZPrepassRenderPass()
{

}

void ZPrepassRenderPass::InitPass()
{
	RendererPtr RendererInstance = Engine::GetRendererInstance();

	int Width = RendererInstance->GetWidth();
	int Height = RendererInstance->GetHeight();

	FrameBufferInstance->SetSize(Width, Height, false);
	FrameBufferInstance->SetColorBuffersCount(0);
	FrameBufferInstance->SetUseDepth(true);
	FrameBufferInstance->SetUseStencil(true);
	FrameBufferInstance->CreateTextures();
	FrameBufferInstance->GenerateBuffers();

	DataManager *DM = DataManager::GetInstance();
	std::string VertexPath = "./src/shaders/src/ZPrepassVertexShader.vs";
	std::string FragmentPath = "./src/shaders/src/ZPrepassFragmentShader.fs";
	std::string ShaderId = VertexPath + FragmentPath;
	DepthShader = DM->RequestResourceByType<Shader, const std::string&, const std::string&>(ShaderId, VertexPath, FragmentPath);
}

void ZPrepassRenderPass::DrawPass()
{
	FrameBufferInstance->Use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glStencilMask(GL_TRUE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, FrameBufferInstance->GetWidth(), FrameBufferInstance->GetHeight());

	ScenePtr Scene = Engine::GetInstance()->GetScene();

	MainCamera = Scene->GetSceneComponent<CameraComponent>();

	View = MainCamera->CalculateViewMatrix();
	Proj = MainCamera->CalculateProjectionMatrix();

	DepthShader->Use();
	DepthShader->SetMat4("view", View);
	DepthShader->SetMat4("projection", Proj);

	// go through mesh components and draw them using assigned materials
	std::vector<MeshComponentPtr> MeshCompVector = Scene->GetSceneComponentsCast<MeshComponent>();
	for (MeshComponentPtr MeshComp : MeshCompVector)
	{
		MeshComp->GetParent()->Transform.SetRotation({ -90.0f, 10.0f * (float)glfwGetTime(), 0.0 });
		Model = MeshComp->GetParent()->Transform.GetMatrix();
		DepthShader->SetMat4("model", Model);
		MeshComp->MeshData->Draw();
	}

	FrameBuffer::Unbind();
}

void ZPrepassRenderPass::OnResolutionChaged(int InWidth, int InHeight)
{
	FrameBufferInstance->SetSize(InWidth, InHeight, true);
}

