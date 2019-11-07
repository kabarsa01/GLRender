#include "TextureCube.h"

#include <stb/stb_image.h>
#include <iostream>

TextureCube::TextureCube(const std::vector<std::string>& InPath, bool InputUsesAlpha, bool InFlipVertical, bool InLinear)
	: Texture( InPath.at(0), InputUsesAlpha, InFlipVertical, InLinear )
	, Path{ InPath }
	, Data {}
{
}

TextureCube::TextureCube(const std::vector<std::string>& InPath)
	: TextureCube( InPath, false, true, false )
{
}

TextureCube::~TextureCube()
{
}

bool TextureCube::Load()
{
	if (Data.size() == 0)
	{
		Data.resize(Path.size());
		for (size_t Index = 0; Index < Path.size(); Index++)
		{
			stbi_set_flip_vertically_on_load(FlipVertical);
			Data[Index] = stbi_load(Path[Index].c_str(), &Width, &Height, &NumChannels, 0);
		}
		SetValid(true);
	}
	return Data.size() > 0;
}

bool TextureCube::Unload()
{
	if (Data.size() > 0)
	{
		for (size_t Index = 0; Index < Data.size(); Index++)
		{
			if (Data[Index] != nullptr) stbi_image_free(Data[Index]);
		}
		Data.clear();
		SetValid(false);
		return true;
	}
	return false;
}

TextureCube::TargetType TextureCube::GetTargetType() const
{
	return TT_Cubemap;
}

unsigned char * TextureCube::GetData(unsigned int InIndex) const
{
	return Data[InIndex];
}

std::string TextureCube::GetPath(unsigned int InIndex)
{
	return Path[InIndex];
}

void TextureCube::OnInitialize()
{
	Texture::OnInitialize();
}

void TextureCube::OnDestroy()
{
	Texture::OnDestroy();
}

void TextureCube::SetupBufferData() const
{
	for (unsigned int Index = 0; Index < 6; Index++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Index, 0, GetInternalFormat(), Width, Height, 0, GetFormat(), GetType(), UseEmpty ? NULL : Data[Index]);
	}
}

TextureCube::TextureCube()
	: Texture( "" )
{
}
