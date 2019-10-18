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
	, MinFiltering ( F_Linear )
	, MagFiltering( F_Linear )
	, WrapU( WM_Tile )
	, WrapV( WM_Tile )
{
}

Texture::~Texture()
{
	DestroyBuffer();
	Unload();
}

bool Texture::Load()
{
	if (Data == nullptr)
	{
		stbi_set_flip_vertically_on_load(FlipVertical);
		Data = stbi_load(Path.c_str(), &Width, &Height, &NumChannels, 0);
		SetValid(Data);
	}
	return Data != nullptr;
}

bool Texture::Unload()
{
	if (Data != nullptr)
	{
		stbi_image_free(Data);
		Data = nullptr;
		SetValid(false);
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetMappedWrap( WrapU ));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetMappedWrap( WrapV ));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetMappedFiltering( MinFiltering ));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetMappedFiltering( MagFiltering ));

		glTexImage2D(GL_TEXTURE_2D, 0, GetInternalFormat(), Width, Height, 0, GetFormat(), GetType(), UseEmpty ? NULL : Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture buffer already exist" << std::endl;
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

void Texture::SetFilteringMode(FilteringMode InFilteringMode, FilteringModeTarget InTarget)
{
	switch (InTarget)
	{
	case FMT_Min:
		MinFiltering = InFilteringMode;
		break;
	case FMT_Mag:
		MagFiltering = InFilteringMode;
		break;
	}
}

Texture::FilteringMode Texture::GetFilteringMode(FilteringModeTarget InTarget)
{
	switch (InTarget)
	{
	case FMT_Min:
		return MinFiltering;
		break;
	case FMT_Mag:
		return MagFiltering;
		break;
	}
	return MinFiltering;
}

void Texture::SetWrapMode(WrapMode InWrapMode, WrapModeTarget InTarget)
{
	switch (InTarget)
	{
	case WMT_U:
		WrapU = InWrapMode;
		break;
	case WMT_V:
		WrapV = InWrapMode;
		break;
	}
}

Texture::WrapMode Texture::GetWrapMode(WrapModeTarget InTarget)
{
	switch (InTarget)
	{
	case WMT_U:
		return WrapU;
		break;
	case WMT_V:
		return WrapV;
		break;
	}
	return WrapU;
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

GLint Texture::GetMappedFiltering(FilteringMode InFilteringMode)
{
	GLint Result;
	switch (InFilteringMode)
	{
	case FilteringMode::F_Nearest:
		Result = GL_NEAREST;
		break;
	case FilteringMode::F_Linear:
		Result = GL_LINEAR;
		break;
	case FilteringMode::F_Nearest_MipmapNearest:
		Result = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case FilteringMode::F_Linear_MipmapNearest:
		Result = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case FilteringMode::F_Nearest_MipmapLinear:
		Result = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case FilteringMode::F_Linear_MipmapLinear:
		Result = GL_LINEAR_MIPMAP_LINEAR;
		break;
	}
	return Result;
}

GLint Texture::GetMappedWrap(WrapMode InWrapMode)
{
	GLint Result;
	switch (InWrapMode)
	{
	case WrapMode::WM_Tile:
		Result = GL_REPEAT;
		break;
	case WrapMode::WM_MirroredTile:
		Result = GL_MIRRORED_REPEAT;
		break;
	case WrapMode::WM_ClampEdge:
		Result = GL_CLAMP_TO_EDGE;
		break;
	case WrapMode::WM_ClampBorder:
		Result = GL_CLAMP_TO_BORDER;
		break;
	}
	return Result;
}

Texture::Texture()
	: Resource( HashString::NONE() )
{
}
