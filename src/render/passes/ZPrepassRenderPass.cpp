#include "./ZPrepassRenderPass.h"
#include "../Renderer.h"
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

	FrameBufferInstance = ObjectBase::NewObject<FrameBuffer>();
	FrameBufferInstance->SetSize(Width, Height, false);
	FrameBufferInstance->GenerateBuffer(0, true, true, true);

	DepthShader = ObjectBase::NewObject<Shader, const std::string&, const std::string&>("./src/shaders/src/ZPrepassVertexShader.vs", "./src/shaders/src/ZPrepassFragmentShader.fs");
	DepthShader->Load();
}

void ZPrepassRenderPass::DrawPass()
{
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	FrameBufferInstance->Use();
	//	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LESS);
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
		MeshComp->GetParent()->Transform.SetRotation({ 10.0f * (float)glfwGetTime(), 0.0f , -90.0f });
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

