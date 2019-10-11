#include "./RenderPass.h"

RenderPass::RenderPass()
	: ObjectBase()
{
}

RenderPass::~RenderPass()
{
}

FrameBufferPtr RenderPass::GetFrameBuffer()
{
	return FrameBufferInstance;
}

void RenderPass::SetResolution(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;
}

void RenderPass::InitPass()
{
}

void RenderPass::DrawPass()
{
}
