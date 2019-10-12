#pragma once

#include "../RenderPass.h"
#include <shaders/Shader.h>
#include <scene/camera/CameraComponent.h>

class ZPrepassRenderPass : public RenderPass
{
public:
	ZPrepassRenderPass(const HashString& InName);
	virtual ~ZPrepassRenderPass();

	// Inherited via RenderPass
	virtual void InitPass() override;
	virtual void DrawPass() override;
	virtual void OnResolutionChaged(int InWidth, int InHeight) override;
protected:
	CameraComponentPtr MainCamera;
	ShaderPtr DepthShader;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Proj;
};

typedef std::shared_ptr<ZPrepassRenderPass> ZPrepassRenderPassPtr;

