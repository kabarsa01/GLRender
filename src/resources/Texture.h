#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "data/Resource.h"
#include <string>
#include <vector>

class Texture : public Resource
{
public:
	// we will limit ourselves to 2d texture and cubemap for now
	enum TargetType
	{
		TT_2D = 0,
		TT_Cubemap
	};
	enum FilteringModeTarget
	{
		FMT_Mag = 0,
		FMT_Min
	};
	enum FilteringMode
	{
		FM_Nearest = 0,
		FM_Linear,
		FM_Nearest_MipmapNearest, // one mip, nearest pixel
		FM_Linear_MipmapNearest, // one mip, linear pixel
		FM_Nearest_MipmapLinear, // two mips, nearest pixels
		FM_Linear_MipmapLinear, // two mips, linear pixels
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
	Texture(const std::vector<std::string>& InPath);
	virtual ~Texture();

	virtual bool Load() override;
	virtual bool Unload() override;
	void InitializeBuffer();
	void DestroyBuffer();

	//	void SetTargetType(TargetType InTarget);
	virtual TargetType GetTargetType() const;
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
	unsigned char* GetData(unsigned int InIndex = 0) const;
	std::string GetPath(unsigned int InIndex = 0);
	bool GetFlipVertical();

	void Use(int InSlotLocation) const;

	virtual void OnInitialize() override;
	virtual void OnDestroy() override;
protected:
	std::vector<std::string> Path;

	//TargetType Target;

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
	std::vector<unsigned char*> Data;
	int Width = 512;
	int Height = 512;
	int NumChannels;

	GLenum GetTarget() const;
	GLint GetInternalFormat() const;
	GLenum GetFormat() const;
	GLenum GetType() const;
	GLint GetMappedFiltering(FilteringMode InFilteringMode) const;
	GLint GetMappedWrap(WrapMode InWrapMode) const;
	virtual void SetupBufferData() const;
private:
	Texture();
};

typedef std::shared_ptr<Texture> TexturePtr;

