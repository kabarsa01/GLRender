#pragma once

#include "core/ObjectBase.h"
#include "render/FrameBuffer.h"

class RenderPass : public ObjectBase
{
public:
	RenderPass();
	virtual ~RenderPass();

	FrameBufferPtr GetFrameBuffer();
	void SetResolution(int InWidth, int InHeight);

	virtual void InitPass();
	virtual void DrawPass();
protected:
	int Width;
	int Height;
	FrameBufferPtr FrameBufferInstance;
};

