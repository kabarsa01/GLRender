#include "ScreenOutputRenderPass.h"

#include <string>

#include "core/Engine.h"

#include "render/Renderer.h"
#include "render/MeshData.h"
#include "render/FrameBuffer.h"

#include "resources/Texture2D.h"

ScreenOutputRenderPass::ScreenOutputRenderPass(const HashString & InName)
	: RenderPass(InName)
{
}

ScreenOutputRenderPass::~ScreenOutputRenderPass()
{
}

void ScreenOutputRenderPass::InitPass()
{
	ScreenQuadShader = ObjectBase::NewObject<Shader, const std::string&, const std::string&>(
		"./src/shaders/src/BasicScreenSpaceVertexShader.vs", 
		"./src/shaders/src/BasicScreenSpaceFragmentShader.fs");
	ScreenQuadShader->Load();
	ScreenQuadShader->Use();
	ScreenQuadShader->SetInt("colorBuffer", 0);

	MeshData::FullscreenQuad()->SetupBufferObjects();
}

void ScreenOutputRenderPass::DrawPass()
{
	FrameBuffer::Unbind();
	RendererPtr Rend = Engine::GetRendererInstance();
	// FINAL PASS
	glViewport(0, 0, Rend->GetWidth(), Rend->GetHeight());

	glStencilMask(GL_FALSE);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ScreenQuadShader->Use();
	
	Rend->GetRenderPass(std::string("MainPass"))->GetFrameBuffer()->GetTexture(0)->Use(0);
	//Rend->GetRenderPass(std::string("ShadowCasters"))->GetFrameBuffer()->GetDepthTexture()->Use(0);
	//Rend->GetRenderPass(std::string("ZPrepass"))->GetFrameBuffer()->GetDepthTexture()->Use(0);
	MeshData::FullscreenQuad()->Draw();
}

void ScreenOutputRenderPass::OnResolutionChaged(int InWidth, int InHeight)
{
}

ScreenOutputRenderPass::ScreenOutputRenderPass()
	: RenderPass( HashString::NONE() )
{
}
