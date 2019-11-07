#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "data/Resource.h"
#include <string>
#include <vector>
#include "Texture.h"

class TextureCube : public Texture
{
public:
	TextureCube(const std::vector<std::string>& InPath, bool InputUsesAlpha = false, bool InFlipVertical = true, bool InLinear = false);
	virtual ~TextureCube();

	virtual bool Load() override;
	virtual bool Unload() override;

	virtual TargetType GetTargetType() const;

	unsigned char* GetData(unsigned int InIndex = 0) const;
	std::string GetPath(unsigned int InIndex = 0);

	virtual void OnInitialize() override;
	virtual void OnDestroy() override;
protected:
	std::vector<std::string> Path;
	std::vector<unsigned char*> Data;

	virtual void SetupBufferData() const;
private:
	TextureCube();
};

typedef std::shared_ptr<TextureCube> TextureCubePtr;

