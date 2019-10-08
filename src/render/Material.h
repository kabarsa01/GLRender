#pragma once

#include <memory>

#include "core/ObjectBase.h"
#include "data/Resource.h"
#include "shaders/Shader.h"
#include "resources/Texture.h"

class Material : public Resource
{
public:
	Material(HashString InId);
	virtual ~Material();

	virtual bool Load() override;
	virtual bool Unload() override;

	void InitializeBuffers();
	void DestroyBuffers();

	void Use();

	std::string VertexShaderPath;
	std::string FragmentShaderPath;
	std::string AlbedoMapPath;
	std::string NormalMapPath;

	ShaderPtr ShaderInstance;
	TexturePtr AlbedoMap;
	TexturePtr NormalMap;
protected:
private:
};

typedef std::shared_ptr<Material> MaterialPtr;

