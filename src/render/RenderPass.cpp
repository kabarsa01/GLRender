#include "./RenderPass.h"

RenderPass::RenderPass()
{
}

RenderPass::~RenderPass()
{
}

FrameBufferPtr RenderPass::GetFrameBuffer()
{
	return FrameBufferInstance;
}

void RenderPass::Draw()
{
}
