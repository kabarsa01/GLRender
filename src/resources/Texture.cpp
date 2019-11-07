#include "Texture.h"

#include <stb/stb_image.h>
#include <iostream>

Texture::Texture(const std::string& InPath, bool InputUsesAlpha, bool InFlipVertical, bool InLinear)
	: Resource( InPath )
	, ID{ (unsigned int)-1 }
	, UseAlpha{ InputUsesAlpha }
	, FlipVertical{ InFlipVertical }
	, Linear{ InLinear }
	, MinFiltering ( FM_Linear_MipmapLinear )
	, MagFiltering( FM_Linear_MipmapLinear )
	, WrapU( WM_Tile )
	, WrapV( WM_Tile )
	, BorderColor( 0.0f, 0.0f, 0.0f, 0.0f )
{
}

Texture::~Texture()
{
	DestroyBuffer();
	Unload();
}

bool Texture::Load()
{
	return false;
}

bool Texture::Unload()
{
	return false;
}

void Texture::InitializeBuffer()
{
	if ( ID == -1 )
	{
		GLenum TextureTarget = GetTarget();
		glGenTextures(1, &ID);
		glBindTexture(TextureTarget, ID);

		glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_S, GetMappedWrap( WrapU ));
		glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_T, GetMappedWrap( WrapV ));
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &BorderColor.r);

		glTexParameteri(TextureTarget, GL_TEXTURE_MIN_FILTER, GetMappedFiltering( MinFiltering ));
		glTexParameteri(TextureTarget, GL_TEXTURE_MAG_FILTER, GetMappedFiltering( MagFiltering ));

		SetupBufferData();

		glGenerateMipmap(TextureTarget);
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

Texture::TargetType Texture::GetTargetType() const
{
	return TT_2D;
}

void Texture::SetSize(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;
}

void Texture::SetUseAlpha(bool InUseAlpha)
{
	UseAlpha = InUseAlpha;
}

void Texture::SetFlipVertical(bool InFlipVertical)
{
	FlipVertical = InFlipVertical;
}

void Texture::SetLinear(bool InLinear)
{
	Linear = InLinear;
}

void Texture::SetUseFloat16(bool InUseFloat16)
{
	UseFloat16 = InUseFloat16;
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

void Texture::SetBorderColor(const glm::vec4 InBorderColor)
{
	BorderColor = InBorderColor;
}

unsigned int Texture::GetID() const
{
	return ID;
}

bool Texture::GetFlipVertical()
{
	return FlipVertical;
}

void Texture::Use(int InSlotLocation) const
{
	if (ID != -1)
	{
		glActiveTexture(GL_TEXTURE0 + InSlotLocation);
		glBindTexture(GetTarget(), ID);
	}
}

void Texture::OnInitialize()
{
	Resource::OnInitialize();
}

void Texture::OnDestroy()
{
	DestroyBuffer();
	Unload();
	Resource::OnDestroy();
}

GLenum Texture::GetTarget() const
{
	switch (GetTargetType())
	{
	case TargetType::TT_2D:
		return GL_TEXTURE_2D;
	case TargetType::TT_Cubemap:
		return GL_TEXTURE_CUBE_MAP;
	}

	return GL_TEXTURE_2D;
}

GLint Texture::GetInternalFormat() const
{
	if (UseDepth)
	{
		return UseStencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT;
	}

	GLint InternalFormat = 0;
	if (Linear)
	{
		InternalFormat = UseFloat16 ? (UseAlpha ? GL_RGBA16F : GL_RGB16F) : (UseAlpha ? GL_RGBA : GL_RGB);
	}
	else
	{
		InternalFormat = UseAlpha ? GL_SRGB_ALPHA : GL_SRGB;
	}
	return InternalFormat;
}

GLenum Texture::GetFormat() const
{
	if (UseDepth)
	{
		return UseStencil ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT;
	}

	return UseAlpha ? GL_RGBA : GL_RGB;
}

GLenum Texture::GetType() const
{
	if (UseDepth)
	{
		return UseStencil ? GL_UNSIGNED_INT_24_8 : GL_FLOAT;
	}
	return UseFloat16 ? GL_FLOAT : GL_UNSIGNED_BYTE;
}

GLint Texture::GetMappedFiltering(FilteringMode InFilteringMode) const
{
	GLint Result;
	switch (InFilteringMode)
	{
	case FilteringMode::FM_Nearest:
		Result = GL_NEAREST;
		break;
	case FilteringMode::FM_Linear:
		Result = GL_LINEAR;
		break;
	case FilteringMode::FM_Nearest_MipmapNearest:
		Result = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case FilteringMode::FM_Linear_MipmapNearest:
		Result = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case FilteringMode::FM_Nearest_MipmapLinear:
		Result = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case FilteringMode::FM_Linear_MipmapLinear:
		Result = GL_LINEAR_MIPMAP_LINEAR;
		break;
	}
	return Result;
}

GLint Texture::GetMappedWrap(WrapMode InWrapMode) const
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

void Texture::SetupBufferData() const
{
}

Texture::Texture()
	: Resource( HashString::NONE() )
{
}
