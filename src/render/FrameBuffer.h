#pragma once

#include <memory>
#include <vector>
#include <glad/glad.h>

#include "core/ObjectBase.h"

class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

class FrameBuffer : public ObjectBase
{
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	void SetSize(int InWidth, int InHeight, bool InRecreateBuffers);
	int GetWidth();
	int GetHeight();
	void SetColorBuffersCount(unsigned int InColorBuffersCount);
	void SetUseDepth(bool InUseDepth);
	void SetUseStencil(bool InUseStencil);
	void CreateTextures();
	void GenerateBuffers();
	void DestroyBuffers();

	void SetTexture(Texture2DPtr InTexture, unsigned int InIndex);
	Texture2DPtr GetTexture(unsigned int InIndex);
	void SetDepthTexture(Texture2DPtr InTexture);
	Texture2DPtr GetDepthTexture();
	void SetAllowDepthReset(bool InAllowDepthReset);

	void Use();
	static void Unbind();

	virtual void OnDestroy() override;
	virtual void OnInitialize() override;

protected:
	unsigned int ID;
	int Width;
	int Height;
	bool UseDepth = true;
	bool UseStencil = true;
	int ColorBuffersCount = 1;
	bool AllowDepthReset = true;

	Texture2DPtr DepthTexture;
	std::vector<Texture2DPtr> Textures;

	Texture2DPtr CreateTexture(size_t InIndex);
	Texture2DPtr CreateDepth();
	void ResetBuffers();
	void SetupAttachments();
};

typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;


