#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <memory>

#include "core/ObjectBase.h"
#include "data/Resource.h"
#include "shaders/Shader.h"
#include "resources/Texture.h"

//=================================================================================
//=================================================================================

struct MaterialTextureRecord
{
public:
	bool InputUsesAlpha;
	bool FlipVertical;
	bool Linear;
	std::string ParamName;
	std::string Path;
	int TextureSlotLocation;
	TexturePtr TextureInstance;
};

//=================================================================================
//=================================================================================

class Material : public Resource
{
public:
	Material(HashString InId);
	virtual ~Material();

	virtual bool Load() override;
	virtual bool Unload() override;

	void InitializeBuffers();
	void DestroyBuffers();

	void Use();

	template<typename T>
	void AddUniformParam(const std::string& InParamName, const T& InParamValue, bool InUpdate = false);
	void AddTextureParam(
		const std::string& InParamName,
		const std::string& InTexturePath,
		int InLocation,
		bool InputUsesAlpha = false,
		bool InFlipVertical = true,
		bool InLinear = true);

	void SetShaderPath(const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath);
	void SetupParams();

	ShaderPtr ShaderInstance;
	TexturePtr AlbedoMap;
	TexturePtr NormalMap;
protected:
	std::string VertexShaderPath;
	std::string FragmentShaderPath;

	std::map<std::string, bool>		BoolParams;
	std::map<std::string, int>		IntParams;
	std::map<std::string, float>	FloatParams;

	std::map<std::string, glm::vec2>	Vec2Params;
	std::map<std::string, glm::vec3>	Vec3Params;
	std::map<std::string, glm::vec4>	Vec4Params;

	std::map<std::string, glm::mat2>	Mat2Params;
	std::map<std::string, glm::mat3>	Mat3Params;
	std::map<std::string, glm::mat4>	Mat4Params;

	std::vector<MaterialTextureRecord> TextureParams;
private:
	Material();
};

typedef std::shared_ptr<Material> MaterialPtr;

//===============================================================================================
// template definitions
//===============================================================================================

template<>
inline void Material::AddUniformParam<bool>(const std::string & InParamName, const bool & InParamValue, bool InUpdate)
{
	BoolParams[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetBool(InParamName, InParamValue);
	}
}

template<>
inline void Material::AddUniformParam<int>(const std::string & InParamName, const int & InParamValue, bool InUpdate)
{
	IntParams[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetInt(InParamName, InParamValue);
	}
}

template<>
inline void Material::AddUniformParam<float>(const std::string & InParamName, const float & InParamValue, bool InUpdate)
{
	FloatParams[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetFloat(InParamName, InParamValue);
	}
}

//---------------------------------------------------------------------------------------------

template<>
inline void Material::AddUniformParam<glm::vec2>(const std::string & InParamName, const glm::vec2 & InParamValue, bool InUpdate)
{
	Vec2Params[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetVec2(InParamName, InParamValue);
	}
}

template<>
inline void Material::AddUniformParam<glm::vec3>(const std::string & InParamName, const glm::vec3 & InParamValue, bool InUpdate)
{
	Vec3Params[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetVec3(InParamName, InParamValue);
	}
}

template<>
inline void Material::AddUniformParam<glm::vec4>(const std::string & InParamName, const glm::vec4 & InParamValue, bool InUpdate)
{
	Vec4Params[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetVec4(InParamName, InParamValue);
	}
}

//---------------------------------------------------------------------------------------------

template<>
inline void Material::AddUniformParam<glm::mat2>(const std::string & InParamName, const glm::mat2 & InParamValue, bool InUpdate)
{
	Mat2Params[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetMat2(InParamName, InParamValue);
	}
}

template<>
inline void Material::AddUniformParam<glm::mat3>(const std::string & InParamName, const glm::mat3 & InParamValue, bool InUpdate)
{
	Mat3Params[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetMat3(InParamName, InParamValue);
	}
}

template<>
inline void Material::AddUniformParam<glm::mat4>(const std::string & InParamName, const glm::mat4 & InParamValue, bool InUpdate)
{
	Mat4Params[InParamName] = InParamValue;
	if (InUpdate)
	{
		ShaderInstance->SetMat4(InParamName, InParamValue);
	}
}


