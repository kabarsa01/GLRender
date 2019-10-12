#pragma once

#include "core/ObjectBase.h"
#include "render/FrameBuffer.h"
#include <common/HashString.h>

class RenderPass : public ObjectBase
{
public:
	RenderPass(const HashString& InName);
	virtual ~RenderPass();

	const HashString& GetName() const;
	FrameBufferPtr GetFrameBuffer();

	virtual void InitPass() = 0;
	virtual void DrawPass() = 0;

	virtual void OnResolutionChaged(int InWidth, int InHeight) = 0;
protected:
	HashString Name;
	FrameBufferPtr FrameBufferInstance;
private:
	RenderPass();
};

typedef std::shared_ptr<RenderPass> RenderPassPtr;

