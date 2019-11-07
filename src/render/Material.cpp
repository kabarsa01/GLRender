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

	for (auto TexParamPair : TextureParams)
	{
		MaterialTextureRecord& Rec = TexParamPair.second;
		if (( !Rec.TextureInstance ) && ( Rec.Path.length() > 0 ))
		{
			Rec.TextureInstance = DM->GetResourceByType<Texture2D>(Rec.Path);
		}
	}

	return true;
}

bool Material::Unload()
{
	return true;
}

void Material::InitializeBuffers()
{
	for (auto TexParamPair : TextureParams)
	{
		const TexturePtr& TextureInstance = TextureParams[TexParamPair.first].TextureInstance;
		if (TextureInstance)
		{
			TextureInstance->SetFilteringMode(Texture::FilteringMode::FM_Linear, Texture::FilteringModeTarget::FMT_Min);
			TextureInstance->SetFilteringMode(Texture::FilteringMode::FM_Linear, Texture::FilteringModeTarget::FMT_Mag);
			TextureInstance->InitializeBuffer();
		}
	}
}

void Material::DestroyBuffers()
{
	for (auto TexParamPair : TextureParams)
	{
		TexParamPair.second.TextureInstance->DestroyBuffer();
	}
}

void Material::Use()
{
	ShaderInstance->Use();

	for (auto TexParamPair : TextureParams)
	{
		MaterialTextureRecord& Rec = TextureParams[TexParamPair.first];
		if (Rec.TextureInstance)
		{
			Rec.TextureInstance->Use(Rec.TextureSlotLocation);
		}
	}
}

void Material::AddTextureParam(const std::string & InParamName, const std::string& InPath, const TexturePtr& InTexture, int InLocation)
{
	MaterialTextureRecord Rec;

	Rec.ParamName = InParamName;
	Rec.Path = InPath;
	Rec.TextureInstance = InTexture;
	Rec.TextureSlotLocation = InLocation;

	TextureParams[InParamName] = Rec;
}

void Material::SetTextureParam(const std::string & InParamName, const std::string& InPath, const TexturePtr & InTexture, int InLocation)
{
	AddTextureParam(InParamName, InPath, InTexture, InLocation);
	ShaderInstance->Use();
	ShaderInstance->SetInt(InParamName, InLocation);
}

void Material::UpdateTextureParam(const std::string & InParamName, const TexturePtr & InTexture, bool InUse)
{
	if (TextureParams.find(InParamName) == TextureParams.end())
	{
		return;
	}

	ShaderInstance->Use();

	MaterialTextureRecord& Rec = TextureParams[InParamName];
	Rec.TextureInstance = InTexture;
	if (InUse && InTexture)
	{
		InTexture->Use(Rec.TextureSlotLocation);
	}
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

	for (auto TexParamPair : TextureParams)
	{
		MaterialTextureRecord& Rec = TextureParams[TexParamPair.first];
		ShaderInstance->SetInt(Rec.ParamName, Rec.TextureSlotLocation);
	}
}

Material::Material()
	: Resource( HashString::NONE() )
{
}
