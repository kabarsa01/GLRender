#pragma once

#include "../RenderPass.h"
#include <common/HashString.h>
#include <glm/fwd.hpp>
#include <shaders/Shader.h>
#include <scene/camera/CameraComponent.h>

class MainRenderPass : public RenderPass
{
public:
	MainRenderPass(const HashString& InName);
	virtual ~MainRenderPass();

	virtual void InitPass() override;
	virtual void DrawPass() override;
	virtual void OnResolutionChaged(int InWidth, int InHeight) override;
protected:
	CameraComponentPtr MainCamera;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Proj;

	void SetupShader(ShaderPtr InShader);
};

typedef std::shared_ptr<MainRenderPass> MainRenderPassPtr;

