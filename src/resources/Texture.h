#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "data/Resource.h"
#include <string>

class Texture : public Resource
{
public:
	Texture(const std::string& InPath, bool InputUsesAlpha = false, bool InFlipVertical = true, bool InLinear = true);
	virtual ~Texture();

	virtual bool Load() override;
	virtual bool Unload() override;
	void InitializeBuffer();
	void DestroyBuffer();

	void SetSize(int Width, int Height);
	void SetUseEmpty(bool InUseEmpty);
	void SetUseDepth(bool InUseDepth);
	void SetUseStencil(bool InUseStencil);
	bool IsUsingStencil();

	unsigned int GetID() const;
	unsigned char* GetData() const;
	std::string GetPath();
	bool GetFlipVertical();

	void Use(GLenum textureUnit) const;
protected:
	std::string Path;
	bool FlipVertical;
	bool UseAlpha;
	bool Linear;
	bool UseEmpty = false;
	bool UseDepth = false;
	bool UseStencil = false;

	unsigned int ID = -1;
	unsigned char* Data;
	int Width;
	int Height;
	int NumChannels;

	GLint GetInternalFormat();
	GLenum GetFormat();
	GLenum GetType();
private:
	Texture();
};

typedef std::shared_ptr<Texture> TexturePtr;

