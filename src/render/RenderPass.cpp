#include "./RenderPass.h"

RenderPass::RenderPass(const HashString& InName)
	: ObjectBase()
	, Name( InName )
{
	FrameBufferInstance = ObjectBase::NewObject<FrameBuffer>();
}

RenderPass::RenderPass()
	: ObjectBase()
	, Name( HashString::NONE() )
{

}

RenderPass::~RenderPass()
{
}

const HashString& RenderPass::GetName() const
{
	return Name;
}

FrameBufferPtr RenderPass::GetFrameBuffer()
{
	return FrameBufferInstance;
}

//void RenderPass::InitPass()
//{
//}
//
//void RenderPass::DrawPass()
//{
//}
