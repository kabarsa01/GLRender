#pragma once

#include <glad/glad.h>

#include "core/ObjectBase.h"

class FrameBuffer : public ObjectBase
{
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	void SetSize(int InWidth, int InHeight);
	void GenerateBuffer(unsigned int InColorBuffersCount, bool InUseDepth, bool InUseStencil);
	void DestroyBuffer();

	void Use();
protected:
	unsigned int ID;
	int Width;
	int Height;
};


