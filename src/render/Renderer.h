#pragma once

#include <memory>
#include <glad/glad.h>
#include <core/ObjectBase.h>

#include "render/MeshData.h"
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

	virtual void Initialize() override;

	void Init();
	void RenderFrame();
protected:
	CameraComponentPtr MainCamera;

	void SetupShader(ShaderPtr InShader);
private:
	//======================= VARS ===============================
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
	//==================== METHODS ===============================
};

typedef std::shared_ptr<Renderer> RendererPtr;

