#include "render/FrameBuffer.h"
#include "resources/Texture.h"

#include <string>
#include <iostream>

FrameBuffer::FrameBuffer()
	: ObjectBase()
	, ID ( 0xffffffff )
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::SetSize(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;
}

int FrameBuffer::GetWidth()
{
	return Width;
}

int FrameBuffer::GetHeight()
{
	return Height;
}

void FrameBuffer::GenerateBuffer(unsigned int InColorBuffersCount/* = 1*/, bool InGenerateTextures/* = false*/, bool InUseDepth/* = false*/)
{
	if (ID != 0xffffffff)
	{
		return;
	}

	ColorBuffersCount = InColorBuffersCount;
	UseDepth = InUseDepth;

	glGenFramebuffers(1, &ID);
	// check if textures should be generated or the ones set from the outside
	// will be used
	if (InGenerateTextures)
	{
		GenerateTextures();
	}
	//GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER variants maybe
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	for (size_t TextureIndex = 0; TextureIndex < Textures.size(); TextureIndex++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + TextureIndex, GL_TEXTURE_2D, Textures[TextureIndex]->GetID(), 0);
	}
	if (UseDepth)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthTexture->GetID(), 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::FRAMEBUFFER:: Framebuffer is complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::DestroyBuffer()
{
	if (ID != 0xffffffff)
	{
		glDeleteFramebuffers(1, &ID);
	}
}

void FrameBuffer::SetTexture(TexturePtr InTexture, unsigned int InIndex)
{
	if (InIndex >= Textures.size())
	{
		Textures.resize(InIndex + 1);
	}
	Textures[InIndex] = InTexture;
}

TexturePtr FrameBuffer::GetTexture(unsigned int InIndex)
{
	return Textures[InIndex];
}

void FrameBuffer::SetDepthTexture(TexturePtr InTexture)
{
	DepthTexture = InTexture;
}

TexturePtr FrameBuffer::GetDepthTexture()
{
	return DepthTexture;
}

void FrameBuffer::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::GenerateTextures()
{
	Textures.clear();
	Textures.resize(ColorBuffersCount);
	for (size_t Index = 0; Index < ColorBuffersCount; Index++)
	{
		// generate texture resource id as a framebuffer id and texture index
		std::string TextureId = std::to_string(ID) + std::to_string(Index);
		TexturePtr Tex = ObjectBase::NewObject<Texture, std::string, bool, bool, bool>(TextureId, false, false, true);
		Tex->SetUseEmpty(true);
		Tex->SetSize(Width, Height);
		Tex->SetUseDepth(false);
		Tex->InitializeBuffer();
		Textures[Index] = Tex;
	}

	if (UseDepth)
	{
		std::string TextureId = std::to_string(ID) + std::to_string(ColorBuffersCount);
		DepthTexture = ObjectBase::NewObject<Texture, std::string, bool, bool, bool>(TextureId, false, false, true);
		DepthTexture->SetUseEmpty(true);
		DepthTexture->SetSize(Width, Height);
		DepthTexture->SetUseDepth(true);
		DepthTexture->InitializeBuffer();
	}
}

