#pragma once

#include <glad/glad.h>
#include <core/ObjectBase.h>

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

	Shader* defaultShader;
	Texture* defaultAlbedo;
	Texture* secondaryAlbedo;
	//==================== METHODS ===============================
};

