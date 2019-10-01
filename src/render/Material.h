#pragma once

#include <memory>

#include "core/ObjectBase.h"
#include "shaders/Shader.h"
#include "resources/Texture.h"

class Material : public ObjectBase
{
public:
	Material();
	virtual ~Material();

	ShaderPtr ShaderInstance;
	TexturePtr Albedo;
	TexturePtr Normal;
protected:
private:
};

typedef std::shared_ptr<Material> MaterialPtr;

