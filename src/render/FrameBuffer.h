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
	void GenerateBuffer(bool InGenerateTextures, bool InGenerateDepth);
	void DestroyBuffer();

	void SetTexture(TexturePtr InTexture, unsigned int InIndex);
	TexturePtr GetTexture(unsigned int InIndex);
	void SetDepthTexture(TexturePtr InTexture);
	TexturePtr GetDepthTexture();
	void SetAllowExternalDepthReset(bool InAllowDepthReset);

	void Use();
	static void Unbind();

	virtual void OnDestroy() override;
protected:
	unsigned int ID;
	int Width;
	int Height;
	bool UseDepth = true;
	bool UseStencil = true;
	bool GenerateDepthFlag;
	bool GenerateTexturesFlag;
	int ColorBuffersCount = 1;
	bool AllowExternalDepthReset = false;

	TexturePtr DepthTexture;
	std::vector<TexturePtr> Textures;

	void GenerateTextures();
	void GenerateDepth();
	void ResetBuffers();
	void SetupAttachments();
};

typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;


