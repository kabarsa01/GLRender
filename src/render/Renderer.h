#pragma once

#include <memory>
#include <glad/glad.h>
#include <core/ObjectBase.h>

#include "render/MeshData.h"
#include "render/FrameBuffer.h"
#include "shaders/Shader.h"
#include "scene/camera/CameraObject.h"
#include "scene/camera/CameraComponent.h"
#include "render/RenderPass.h"

class Shader;
class Texture;
class MeshObject;
class CameraObject;

class Renderer : public ObjectBase
{
public:
	Renderer();
	virtual ~Renderer();

	virtual void OnInitialize() override;

	void Init();
	void RenderFrame();

	void SetResolution(int InWidth, int InHeight);
	int GetWidth() const;
	int GetHeight() const;

	RenderPassPtr GetRenderPass(const std::string& InName);
	RenderPassPtr GetRenderPass(const HashString& InName);
protected:
	CameraComponentPtr MainCamera;
private:
	//======================= VARS ===============================
	int Width = 1280;
	int Height = 720;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Proj;

	std::vector<RenderPassPtr> RenderPasses;
	std::map<HashString, unsigned int> RenderPassMap;
	//==================== METHODS ===============================

	void RegisterRenderPass(RenderPassPtr InRenderPass);
};

typedef std::shared_ptr<Renderer> RendererPtr;

