#include "Material.h"
#include "data/DataManager.h"

Material::Material(HashString InId)
	: Resource(InId)
{
}

Material::~Material()
{
}

bool Material::Load()
{
	DataManager *DM = DataManager::GetInstance();
	ShaderInstance = DM->RequestResourceByType<Shader, const std::string&, const std::string&>(VertexShaderPath + FragmentShaderPath, VertexShaderPath, FragmentShaderPath);

	for (size_t Index = 0; Index < TextureParams.size(); Index++)
	{
		MaterialTextureRecord& Rec = TextureParams[Index];
		Rec.TextureInstance = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(
			Rec.Path, 
			Rec.Path, 
			Rec.InputUsesAlpha, 
			Rec.FlipVertical, 
			Rec.Linear);
	}
	//AlbedoMap = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(AlbedoMapPath, AlbedoMapPath, false, true, false);
	//NormalMap = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(NormalMapPath, NormalMapPath, false, true, true);

	return true;
}

bool Material::Unload()
{
	return true;
}

void Material::InitializeBuffers()
{
	for (size_t Index = 0; Index < TextureParams.size(); Index++)
	{
		TextureParams[Index].TextureInstance->InitializeBuffer();
	}
	//AlbedoMap->InitializeBuffer();
	//NormalMap->InitializeBuffer();
}

void Material::DestroyBuffers()
{
	for (size_t Index = 0; Index < TextureParams.size(); Index++)
	{
		TextureParams[Index].TextureInstance->DestroyBuffer();
	}
}

void Material::Use()
{
	ShaderInstance->Use();

	for (size_t Index = 0; Index < TextureParams.size(); Index++)
	{
		MaterialTextureRecord& Rec = TextureParams[Index];
		Rec.TextureInstance->Use(GL_TEXTURE0 + Rec.TextureSlotLocation);
	}
}

void Material::AddTextureParam(
	const std::string & InParamName, 
	const std::string& InTexturePath, 
	int InLocation, 
	bool InputUsesAlpha/* = false*/, 
	bool InFlipVertical/* = true*/, 
	bool InLinear/* = true*/)
{
	MaterialTextureRecord Rec;

	Rec.InputUsesAlpha = InputUsesAlpha;
	Rec.FlipVertical = InFlipVertical;
	Rec.Linear = InLinear;
	Rec.ParamName = InParamName;
	Rec.Path = InTexturePath;
	Rec.TextureSlotLocation = InLocation;

	TextureParams.push_back(Rec);
}

void Material::SetShaderPath(const std::string & InVertexShaderPath, const std::string & InFragmentShaderPath)
{
	VertexShaderPath = InVertexShaderPath;
	FragmentShaderPath = InFragmentShaderPath;
}

void Material::SetupParams()
{
	ShaderInstance->Use();

	for (auto Pair : BoolParams)
	{
		ShaderInstance->SetBool(Pair.first, Pair.second);
	}
	for (auto Pair : IntParams)
	{
		ShaderInstance->SetInt(Pair.first, Pair.second);
	}
	for (auto Pair : FloatParams)
	{
		ShaderInstance->SetFloat(Pair.first, Pair.second);
	}

	for (auto Pair : Vec2Params)
	{
		ShaderInstance->SetVec2(Pair.first, Pair.second);
	}
	for (auto Pair : Vec3Params)
	{
		ShaderInstance->SetVec3(Pair.first, Pair.second);
	}
	for (auto Pair : Vec4Params)
	{
		ShaderInstance->SetVec4(Pair.first, Pair.second);
	}

	for (auto Pair : Mat2Params)
	{
		ShaderInstance->SetMat2(Pair.first, Pair.second);
	}
	for (auto Pair : Mat3Params)
	{
		ShaderInstance->SetMat3(Pair.first, Pair.second);
	}
	for (auto Pair : Mat4Params)
	{
		ShaderInstance->SetMat4(Pair.first, Pair.second);
	}

	for (size_t Index = 0; Index < TextureParams.size(); Index++)
	{
		MaterialTextureRecord& Rec = TextureParams[Index];
		ShaderInstance->SetInt(Rec.ParamName, Rec.TextureSlotLocation);
	}
}

Material::Material()
	: Resource( HashString::NONE() )
{
}
