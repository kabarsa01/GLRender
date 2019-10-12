#include "render/FrameBuffer.h"
#include "data/DataManager.h"
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

void FrameBuffer::SetSize(int InWidth, int InHeight, bool InRecreateBuffers)
{
	Width = InWidth;
	Height = InHeight;
	if (InRecreateBuffers)
	{
		ResetBuffers();
		SetupAttachments();
		//DestroyBuffer();
		//GenerateBuffer(ColorBuffersCount, UseGeneratedTextures, UseDepth, UseGeneratedDepth);
	}
}

int FrameBuffer::GetWidth()
{
	return Width;
}

int FrameBuffer::GetHeight()
{
	return Height;
}

void FrameBuffer::GenerateBuffer(unsigned int InColorBuffersCount/* = 1*/, bool InGenerateTextures/* = false*/, bool InUseDepth/* = false*/, bool InGenerateDepth/* = false*/)
{
	if (ID != 0xffffffff)
	{
		return;
	}

	ColorBuffersCount = InColorBuffersCount;
	UseGeneratedTextures = InGenerateTextures;
	UseDepth = InUseDepth;
	UseGeneratedDepth = InGenerateDepth;

	glGenFramebuffers(1, &ID);
	// check if textures should be generated or the ones set from the outside
	// will be used
	if (InGenerateTextures)
	{
		GenerateTextures();
	}
	if (UseDepth && InGenerateDepth)
	{
		GenerateDepth();
	}
	//GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER variants maybe
	SetupAttachments();

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
		ID = 0xffffffff;

		if (UseGeneratedTextures)
		{
			for (size_t TexIndex = 0; TexIndex < Textures.size(); TexIndex++)
			{
				DataManager::GetInstance()->DeleteResource(Textures[TexIndex]);
				Textures[TexIndex]->DestroyBuffer();
			}
			Textures.clear();
			if (DepthTexture)
			{
				DataManager::GetInstance()->DeleteResource(DepthTexture);
				DepthTexture->DestroyBuffer();
				DepthTexture = nullptr;
			}
		}
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

void FrameBuffer::OnDestroy()
{
	DestroyBuffer();
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
}

void FrameBuffer::GenerateDepth()
{
	std::string TextureId = std::to_string(ID) + std::to_string(ColorBuffersCount);
	DepthTexture = ObjectBase::NewObject<Texture, std::string, bool, bool, bool>(TextureId, false, false, true);
	DepthTexture->SetUseEmpty(true);
	DepthTexture->SetSize(Width, Height);
	DepthTexture->SetUseDepth(true);
	DepthTexture->InitializeBuffer();
}

void FrameBuffer::ResetBuffers()
{
	for (size_t Index = 0; Index < ColorBuffersCount; Index++)
	{
		TexturePtr Tex = Textures[Index];
		Tex->SetSize(Width, Height);
		Tex->DestroyBuffer();
		Tex->InitializeBuffer();
	}

	if (DepthTexture && UseGeneratedDepth)
	{
		DepthTexture->SetSize(Width, Height);
		DepthTexture->DestroyBuffer();
		DepthTexture->InitializeBuffer();
	}
}

void FrameBuffer::SetupAttachments()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	for (unsigned int TextureIndex = 0; TextureIndex < Textures.size(); TextureIndex++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + TextureIndex, GL_TEXTURE_2D, Textures[TextureIndex]->GetID(), 0);
	}
	if (UseDepth)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthTexture->GetID(), 0);
	}
}

