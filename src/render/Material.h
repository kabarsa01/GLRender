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
	void AddUniformParam(const std::string& InParamName, const T& InParamValue);
	template<typename T>
	void SetUniformParam(const std::string& InParamName, const T& InParamValue);
	void AddTextureParam(const std::string& InParamName, const std::string& InPath, const TexturePtr& InTexture, int InLocation);
	void SetTextureParam(const std::string& InParamName, const std::string& InPath, const TexturePtr& InTexture, int InLocation);
	void UpdateTextureParam(const std::string& InParamName, const TexturePtr& InTexture, bool InUse);

	void SetShaderPath(const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath);
	void SetupParams();

	ShaderPtr ShaderInstance;
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

	std::map<std::string, MaterialTextureRecord> TextureParams;
private:
	Material();
};

typedef std::shared_ptr<Material> MaterialPtr;

//===============================================================================================
// template definitions
//===============================================================================================

template<>
inline void Material::AddUniformParam<bool>(const std::string & InParamName, const bool & InParamValue)
{
	BoolParams[InParamName] = InParamValue;
}

template<>
inline void Material::AddUniformParam<int>(const std::string & InParamName, const int & InParamValue)
{
	IntParams[InParamName] = InParamValue;
}

template<>
inline void Material::AddUniformParam<float>(const std::string & InParamName, const float & InParamValue)
{
	FloatParams[InParamName] = InParamValue;
}

//---------------------------------------------------------------------------------------------

template<>
inline void Material::AddUniformParam<glm::vec2>(const std::string & InParamName, const glm::vec2 & InParamValue)
{
	Vec2Params[InParamName] = InParamValue;
}

template<>
inline void Material::AddUniformParam<glm::vec3>(const std::string & InParamName, const glm::vec3 & InParamValue)
{
	Vec3Params[InParamName] = InParamValue;
}

template<>
inline void Material::AddUniformParam<glm::vec4>(const std::string & InParamName, const glm::vec4 & InParamValue)
{
	Vec4Params[InParamName] = InParamValue;
}

//---------------------------------------------------------------------------------------------

template<>
inline void Material::AddUniformParam<glm::mat2>(const std::string & InParamName, const glm::mat2 & InParamValue)
{
	Mat2Params[InParamName] = InParamValue;
}

template<>
inline void Material::AddUniformParam<glm::mat3>(const std::string & InParamName, const glm::mat3 & InParamValue)
{
	Mat3Params[InParamName] = InParamValue;
}

template<>
inline void Material::AddUniformParam<glm::mat4>(const std::string & InParamName, const glm::mat4 & InParamValue)
{
	Mat4Params[InParamName] = InParamValue;
}

//===============================================================================================
// template definitions
//===============================================================================================

template<>
inline void Material::SetUniformParam<bool>(const std::string & InParamName, const bool & InParamValue)
{
	AddUniformParam<bool>(InParamName, InParamValue);
	ShaderInstance->SetBool(InParamName, InParamValue);
}

template<>
inline void Material::SetUniformParam<int>(const std::string & InParamName, const int & InParamValue)
{
	AddUniformParam<int>(InParamName, InParamValue);
	ShaderInstance->SetInt(InParamName, InParamValue);
}

template<>
inline void Material::SetUniformParam<float>(const std::string & InParamName, const float & InParamValue)
{
	AddUniformParam<float>(InParamName, InParamValue);
	ShaderInstance->SetFloat(InParamName, InParamValue);
}

//---------------------------------------------------------------------------------------------

template<>
inline void Material::SetUniformParam<glm::vec2>(const std::string & InParamName, const glm::vec2 & InParamValue)
{
	AddUniformParam<glm::vec2>(InParamName, InParamValue);
	ShaderInstance->SetVec2(InParamName, InParamValue);
}

template<>
inline void Material::SetUniformParam<glm::vec3>(const std::string & InParamName, const glm::vec3 & InParamValue)
{
	AddUniformParam<glm::vec3>(InParamName, InParamValue);
	ShaderInstance->SetVec3(InParamName, InParamValue);
}

template<>
inline void Material::SetUniformParam<glm::vec4>(const std::string & InParamName, const glm::vec4 & InParamValue)
{
	AddUniformParam<glm::vec4>(InParamName, InParamValue);
	ShaderInstance->SetVec4(InParamName, InParamValue);
}

//---------------------------------------------------------------------------------------------

template<>
inline void Material::SetUniformParam<glm::mat2>(const std::string & InParamName, const glm::mat2 & InParamValue)
{
	AddUniformParam<glm::mat2>(InParamName, InParamValue);
	ShaderInstance->SetMat2(InParamName, InParamValue);
}

template<>
inline void Material::SetUniformParam<glm::mat3>(const std::string & InParamName, const glm::mat3 & InParamValue)
{
	AddUniformParam<glm::mat3>(InParamName, InParamValue);
	ShaderInstance->SetMat3(InParamName, InParamValue);
}

template<>
inline void Material::SetUniformParam<glm::mat4>(const std::string & InParamName, const glm::mat4 & InParamValue)
{
	AddUniformParam<glm::mat4>(InParamName, InParamValue);
	ShaderInstance->SetMat4(InParamName, InParamValue);
}



