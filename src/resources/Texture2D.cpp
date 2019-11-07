#include "Texture2D.h"

#include <stb/stb_image.h>
#include <iostream>

Texture2D::Texture2D(const std::string& InPath, bool InputUsesAlpha, bool InFlipVertical, bool InLinear)
	: Texture( InPath, InputUsesAlpha, InFlipVertical, InLinear )
	, Path{ InPath }
	, Data ( nullptr )
{
}

Texture2D::Texture2D(const std::string& InPath)
	: Texture2D( InPath, false, true, false )
{

}

Texture2D::~Texture2D()
{
}

bool Texture2D::Load()
{
	if (Data == nullptr)
	{
		stbi_set_flip_vertically_on_load(FlipVertical);
		Data = stbi_load(Path.c_str(), &Width, &Height, &NumChannels, 0);
		SetValid(Data != nullptr);
	}
	return Data != nullptr;
}

bool Texture2D::Unload()
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

Texture2D::TargetType Texture2D::GetTargetType() const
{
	return TT_2D;
}

unsigned char * Texture2D::GetData() const
{
	return Data;
}

std::string Texture2D::GetPath()
{
	return Path;
}

void Texture2D::OnInitialize()
{
	Texture::OnInitialize();
}

void Texture2D::OnDestroy()
{
	Texture::OnDestroy();
}

void Texture2D::SetupBufferData() const
{
	glTexImage2D(GL_TEXTURE_2D, 0, GetInternalFormat(), Width, Height, 0, GetFormat(), GetType(), UseEmpty ? NULL : Data);
}

Texture2D::Texture2D()
	: Texture( "" )
{
}
