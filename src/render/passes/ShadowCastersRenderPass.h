#pragma once

#include "../RenderPass.h"
#include <common/HashString.h>
#include <glm/fwd.hpp>
#include <shaders/Shader.h>
#include <scene/light/LightComponent.h>

class ShadowCastersRenderPass : public RenderPass
{
public:
	ShadowCastersRenderPass(const HashString& InName);
	virtual ~ShadowCastersRenderPass();

	virtual void InitPass() override;
	virtual void DrawPass() override;
	virtual void OnResolutionChaged(int InWidth, int InHeight) override;
protected:
	ShaderPtr DepthShader;
	LightComponentPtr MainLight;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Proj;
};

typedef std::shared_ptr<ShadowCastersRenderPass> ShadowCastersRenderPassPtr;

