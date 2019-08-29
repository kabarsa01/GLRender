#pragma once

#include <memory>
#include <glad/glad.h>
#include <core/ObjectBase.h>

#include "scene/mesh/MeshObject.h"

class Shader;
class Texture;

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
	//==================== METHODS ===============================
};

