#pragma once

#include <memory>

#include "render/RenderPass.h"
#include "common/HashString.h"
#include "shaders/Shader.h"

class ScreenOutputRenderPass : public RenderPass
{
public:
	ScreenOutputRenderPass(const HashString& InName);
	virtual ~ScreenOutputRenderPass();

	// Inherited via RenderPass
	virtual void InitPass() override;
	virtual void DrawPass() override;
	virtual void OnResolutionChaged(int InWidth, int InHeight) override;
protected:
	ShaderPtr ScreenQuadShader;
private:
	ScreenOutputRenderPass();
};

typedef std::shared_ptr<ScreenOutputRenderPass> ScreenOutputRenderPassPtr;

