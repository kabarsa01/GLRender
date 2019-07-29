#include "Texture.h"

#include <stb/stb_image.h>
#include <iostream>

Texture::Texture(const char * path, bool InputUsesAlpha)
	: ID(-1)
{
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path, &width, &height, &numChannels, 0);

	if (data)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, InputUsesAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}


Texture::~Texture()
{
}

unsigned int Texture::GetID() const
{
	return ID;
}

unsigned char * Texture::GetData() const
{
	return data;
}

void Texture::Use(GLenum textureUnit) const
{
	if (ID != -1)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}
}
