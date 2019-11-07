#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "data/Resource.h"
#include "resources/Texture.h"
#include <string>
#include <vector>

class Texture2D : public Texture
{
public:
	Texture2D(const std::string& InPath, bool InputUsesAlpha = false, bool InFlipVertical = true, bool InLinear = true);
	Texture2D(const std::string& InPath);
	virtual ~Texture2D();

	virtual bool Load() override;
	virtual bool Unload() override;

	virtual TargetType GetTargetType() const;

	unsigned char* GetData() const;
	std::string GetPath();

	virtual void OnInitialize() override;
	virtual void OnDestroy() override;
protected:
	std::string Path;
	unsigned char* Data;

	virtual void SetupBufferData() const;
private:
	Texture2D();
};

typedef std::shared_ptr<Texture2D> Texture2DPtr;

