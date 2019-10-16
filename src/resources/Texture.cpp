#include "Texture.h"

#include <stb/stb_image.h>
#include <iostream>

Texture::Texture(const std::string& InPath, bool InputUsesAlpha, bool InFlipVertical, bool InLinear)
	: Resource{ InPath }
	, ID{ (unsigned int)-1 }
	, Path{ InPath }
	, UseAlpha{ InputUsesAlpha }
	, FlipVertical{ InFlipVertical }
	, Linear{ InLinear }
	, Data ( nullptr )
{
}

Texture::~Texture()
{
	DestroyBuffer();
	Unload();
}

bool Texture::Load()
{
	stbi_set_flip_vertically_on_load(FlipVertical);
	Data = stbi_load(Path.c_str(), &Width, &Height, &NumChannels, 0);
	SetValid(Data);
	return Data != nullptr;
}

bool Texture::Unload()
{
	if (Data != nullptr)
	{
		stbi_image_free(Data);
		Data = nullptr;
		return true;
	}
	return false;
}

void Texture::InitializeBuffer()
{
	if ( ID == -1 )
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		GLint MinFilterParam = UseStencil ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST;
		GLint MagFilterParam = UseStencil ? GL_LINEAR : GL_NEAREST;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterParam);

		glTexImage2D(GL_TEXTURE_2D, 0, GetInternalFormat(), Width, Height, 0, GetFormat(), GetType(), UseEmpty ? NULL : Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

void Texture::DestroyBuffer()
{
	if (ID != -1)
	{
		glDeleteTextures(1, &ID);
		ID = -1;
	}
}

void Texture::SetSize(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;
}

void Texture::SetUseEmpty(bool InUseEmpty)
{
	UseEmpty = InUseEmpty;
}

void Texture::SetUseDepth(bool InUseDepth)
{
	UseDepth = InUseDepth;
}

void Texture::SetUseStencil(bool InUseStencil)
{
	UseStencil = InUseStencil;
}

bool Texture::IsUsingStencil()
{
	return UseStencil;
}

unsigned int Texture::GetID() const
{
	return ID;
}

unsigned char * Texture::GetData() const
{
	return Data;
}

std::string Texture::GetPath()
{
	return Path;
}

bool Texture::GetFlipVertical()
{
	return FlipVertical;
}

void Texture::Use(GLenum textureUnit) const
{
	if (ID != -1)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}
}

GLint Texture::GetInternalFormat()
{
	if (UseDepth)
	{
		return UseStencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT;
	}

	GLint InternalFormat = 0;
	if (Linear)
	{
		InternalFormat = UseAlpha ? GL_RGBA : GL_RGB;
	}
	else
	{
		InternalFormat = UseAlpha ? GL_SRGB_ALPHA : GL_SRGB;
	}
	return InternalFormat;
}

GLenum Texture::GetFormat()
{
	if (UseDepth)
	{
		return UseStencil ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT;
	}

	return UseAlpha ? GL_RGBA : GL_RGB;
}

GLenum Texture::GetType()
{
	return UseDepth ? (UseStencil ? GL_UNSIGNED_INT_24_8 : GL_FLOAT) : GL_UNSIGNED_BYTE;
}

Texture::Texture()
	: Resource( HashString::NONE() )
{
}
