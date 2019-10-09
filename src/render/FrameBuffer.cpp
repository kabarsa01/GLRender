#include "render/FrameBuffer.h"

FrameBuffer::FrameBuffer()
	: ObjectBase()
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

void FrameBuffer::GenerateBuffer(unsigned int InColorBuffersCount, bool InUseDepth, bool InUseStencil)
{
	glGenFramebuffers(1, &ID);
}

void FrameBuffer::DestroyBuffer()
{
	glDeleteFramebuffers(1, &ID);
}

void FrameBuffer::Use()
{
}

