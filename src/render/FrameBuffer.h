#pragma once

#include <memory>
#include <vector>
#include <glad/glad.h>

#include "core/ObjectBase.h"

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

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

	void SetTexture(TexturePtr InTexture, unsigned int InIndex);
	TexturePtr GetTexture(unsigned int InIndex);
	void SetDepthTexture(TexturePtr InTexture);
	TexturePtr GetDepthTexture();
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

	TexturePtr DepthTexture;
	std::vector<TexturePtr> Textures;

	TexturePtr CreateTexture(size_t InIndex);
	TexturePtr CreateDepth();
	void ResetBuffers();
	void SetupAttachments();
};

typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;


