#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "data/Resource.h"
#include <string>

class Texture : public Resource
{
public:
	enum FilteringModeTarget
	{
		FMT_Mag = 0,
		FMT_Min
	};
	enum FilteringMode
	{
		F_Nearest = 0,
		F_Linear,
		F_Nearest_MipmapNearest, // one mip, nearest pixel
		F_Linear_MipmapNearest, // one mip, linear pixel
		F_Nearest_MipmapLinear, // two mips, nearest pixels
		F_Linear_MipmapLinear, // two mips, linear pixels
	};
	enum WrapModeTarget
	{
		WMT_U = 0,
		WMT_V
	};
	enum WrapMode
	{
		WM_Tile = 0,
		WM_MirroredTile,
		WM_ClampEdge,
		WM_ClampBorder
	};

	Texture(const std::string& InPath, bool InputUsesAlpha = false, bool InFlipVertical = true, bool InLinear = true);
	Texture(const std::string& InPath);
	virtual ~Texture();

	virtual bool Load() override;
	virtual bool Unload() override;
	void InitializeBuffer();
	void DestroyBuffer();

	void SetSize(int Width, int Height);
	void SetUseAlpha(bool InUseAlpha);
	void SetFlipVertical(bool InFlipVertical);
	void SetLinear(bool InLinear);
	void SetUseFloat16(bool InUseFloat16);
	void SetUseEmpty(bool InUseEmpty);
	void SetUseDepth(bool InUseDepth);
	void SetUseStencil(bool InUseStencil);
	bool IsUsingStencil();
	void SetFilteringMode(FilteringMode InFilteringMode, FilteringModeTarget InTarget);
	FilteringMode GetFilteringMode(FilteringModeTarget InTarget);
	void SetWrapMode(WrapMode InWrapMode, WrapModeTarget InTarget);
	WrapMode GetWrapMode(WrapModeTarget InTarget);
	void SetBorderColor(const glm::vec4 InBorderColor);

	unsigned int GetID() const;
	unsigned char* GetData() const;
	std::string GetPath();
	bool GetFlipVertical();

	void Use(int InSlotLocation) const;

	virtual void OnInitialize() override;
	virtual void OnDestroy() override;
protected:
	std::string Path;
	bool FlipVertical = true;
	bool UseAlpha = false;
	bool Linear = false;
	bool UseFloat16 = false;
	bool UseEmpty = false;
	bool UseDepth = false;
	bool UseStencil = false;
	glm::vec4 BorderColor;

	FilteringMode MinFiltering;
	FilteringMode MagFiltering;
	WrapMode WrapU;
	WrapMode WrapV;

	unsigned int ID = -1;
	unsigned char* Data;
	int Width = 512;
	int Height = 512;
	int NumChannels;

	GLint GetInternalFormat();
	GLenum GetFormat();
	GLenum GetType();
	GLint GetMappedFiltering(FilteringMode InFilteringMode);
	GLint GetMappedWrap(WrapMode InWrapMode);
private:
	Texture();
};

typedef std::shared_ptr<Texture> TexturePtr;

