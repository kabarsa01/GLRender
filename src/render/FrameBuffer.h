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
	void GenerateBuffer(unsigned int InColorBuffersCount, bool InGenerateTextures, bool InUseDepth, bool InGenerateDepth);
	void DestroyBuffer();

	void SetTexture(TexturePtr InTexture, unsigned int InIndex);
	TexturePtr GetTexture(unsigned int InIndex);
	void SetDepthTexture(TexturePtr InTexture);
	TexturePtr GetDepthTexture();

	void Use();
	static void Unbind();

	virtual void OnDestroy() override;
protected:
	unsigned int ID;
	int Width;
	int Height;
	bool UseDepth;
	bool UseGeneratedDepth;
	bool UseGeneratedTextures;
	int ColorBuffersCount = 1;

	TexturePtr DepthTexture;
	std::vector<TexturePtr> Textures;

	void GenerateTextures();
	void GenerateDepth();
	void ResetBuffers();
	void SetupAttachments();
};

typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;


