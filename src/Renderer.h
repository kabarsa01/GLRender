#pragma once

#include <memory>
#include <glad/glad.h>
#include <core/ObjectBase.h>

class Shader;
class Texture;
class MeshObject;
class CameraObject;

class Renderer : public ObjectBase
{
public:
	Renderer();
	virtual ~Renderer();

	void Init();
	void RenderFrame();
private:
	//======================= VARS ===============================
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	std::shared_ptr<Shader> DefaultShader;
	std::shared_ptr<Texture> DefaultAlbedo;
	std::shared_ptr<Texture> SecondaryAlbedo;

	// test
	std::shared_ptr<MeshObject> MeshObj;
	std::shared_ptr<CameraObject> CameraObj;
	//==================== METHODS ===============================
};

