#include "render/FrameBuffer.h"
#include "data/DataManager.h"
#include "resources/Texture.h"

#include <string>
#include <iostream>

FrameBuffer::FrameBuffer()
	: ObjectBase()
	, ID ( 0xffffffff )
	, DepthTexture( nullptr )
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

void FrameBuffer::SetColorBuffersCount(unsigned int InColorBuffersCount)
{
	ColorBuffersCount = InColorBuffersCount;
}

void FrameBuffer::SetUseDepth(bool InUseDepth)
{
	UseDepth = InUseDepth;
}

void FrameBuffer::SetUseStencil(bool InUseStencil)
{
	UseStencil = InUseStencil;
}

void FrameBuffer::CreateTextures()
{
	Textures.resize(ColorBuffersCount);
	for (size_t Index = 0; Index < ColorBuffersCount; Index++)
	{
		if (Textures[Index])
		{
			continue;
		}
		Textures[Index] = CreateTexture(Index);
	}

	if (!DepthTexture && UseDepth)
	{
		DepthTexture = CreateDepth();
	}
}

void FrameBuffer::GenerateBuffers()
{
	// check if textures should be generated or the ones set from the outside
	// will be used
	for (size_t Index = 0; Index < ColorBuffersCount; Index++)
	{
		if (!Textures[Index])
		{
			Textures[Index] = CreateTexture(Index);
		}
		Textures[Index]->InitializeBuffer();
	}

	if (!DepthTexture && UseDepth)
	{
		DepthTexture = CreateDepth();
	}

	if (DepthTexture)
	{
		DepthTexture->InitializeBuffer();
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

void FrameBuffer::DestroyBuffers()
{
	if (ID != 0xffffffff)
	{
		glDeleteFramebuffers(1, &ID);
		ID = 0xffffffff;

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

void FrameBuffer::SetAllowDepthReset(bool InAllowDepthReset)
{
	AllowDepthReset = InAllowDepthReset;
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
	DestroyBuffers();
}

void FrameBuffer::OnInitialize()
{
	if (ID != 0xffffffff)
	{
		return;
	}

	glGenFramebuffers(1, &ID);
}

TexturePtr FrameBuffer::CreateTexture(size_t InIndex)
{
	std::string TextureId = std::to_string(ID) + std::to_string(InIndex);
	TexturePtr Tex = ObjectBase::NewObject<Texture, std::string, bool, bool, bool>(TextureId, false, false, true);
	Tex->SetUseEmpty(true);
	Tex->SetSize(Width, Height);
	Tex->SetUseDepth(false);
	Tex->SetUseFloat16(true);
	Tex->SetFilteringMode(Texture::FilteringMode::FM_Linear_MipmapLinear, Texture::FilteringModeTarget::FMT_Min);
	Tex->SetFilteringMode(Texture::FilteringMode::FM_Linear_MipmapLinear, Texture::FilteringModeTarget::FMT_Mag);
	return Tex;
}

TexturePtr FrameBuffer::CreateDepth()
{
	std::string TextureId = std::to_string(ID) + std::to_string(ColorBuffersCount);
	TexturePtr Tex = ObjectBase::NewObject<Texture, std::string, bool, bool, bool>(TextureId, false, false, true);
	Tex->SetUseEmpty(true);
	Tex->SetSize(Width, Height);
	Tex->SetUseDepth(true);
	Tex->SetUseStencil(UseStencil);
	Tex->SetFilteringMode(Texture::FilteringMode::FM_Linear, Texture::FilteringModeTarget::FMT_Min);
	Tex->SetFilteringMode(Texture::FilteringMode::FM_Linear, Texture::FilteringModeTarget::FMT_Mag);
	return Tex;
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

	if (DepthTexture && AllowDepthReset)
	{
		DepthTexture->SetSize(Width, Height);
		DepthTexture->DestroyBuffer();
		DepthTexture->InitializeBuffer();
	}
}

void FrameBuffer::SetupAttachments()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	std::vector<unsigned int> Attachments(Textures.size());
	for (unsigned int TextureIndex = 0; TextureIndex < Textures.size(); TextureIndex++)
	{
		unsigned int AttachmentType = GL_COLOR_ATTACHMENT0 + TextureIndex;
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentType, GL_TEXTURE_2D, Textures[TextureIndex]->GetID(), 0);
		Attachments[TextureIndex] = AttachmentType;
	}
	// tell OpenGL which color attachments we'll use for rendering 
	glDrawBuffers(Attachments.size(), Attachments.data());

	if (UseDepth)
	{
		GLenum Attachment = DepthTexture->IsUsingStencil() ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, DepthTexture->GetID(), 0);
	}
}

