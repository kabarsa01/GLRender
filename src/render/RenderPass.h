#pragma once

#include "core/ObjectBase.h"
#include "render/FrameBuffer.h"

class RenderPass : public ObjectBase
{
public:
	RenderPass();
	virtual ~RenderPass();

	FrameBufferPtr GetFrameBuffer();

	virtual void Draw();
protected:
	FrameBufferPtr FrameBufferInstance;
};

