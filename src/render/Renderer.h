#pragma once

#include <memory>
#include <glad/glad.h>
#include <core/ObjectBase.h>

#include "render/MeshData.h"
#include "render/FrameBuffer.h"
#include "shaders/Shader.h"
#include "scene/camera/CameraObject.h"
#include "scene/camera/CameraComponent.h"

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
protected:
	CameraComponentPtr MainCamera;

	void SetupShader(ShaderPtr InShader);
private:
	//======================= VARS ===============================
	int Width = 1280;
	int Height = 720;

	std::shared_ptr<Shader> DefaultShader;
	std::shared_ptr<Texture> DefaultAlbedo;
	std::shared_ptr<Texture> SecondaryAlbedo;

	// test
	std::vector<std::shared_ptr<MeshObject>> MeshObjects;
	std::vector<std::shared_ptr<Texture>> Albedos;
	std::vector<std::shared_ptr<Texture>> NormalMaps;
	std::shared_ptr<MeshObject> MeshObj;
	std::shared_ptr<CameraObject> CameraObj;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Proj;

	FrameBufferPtr PrimaryFrameBuffer;
	ShaderPtr SSShader;
	MeshDataPtr ScreenQuad;
	//==================== METHODS ===============================
};

typedef std::shared_ptr<Renderer> RendererPtr;

