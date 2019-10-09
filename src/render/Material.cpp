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
	AlbedoMap = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(AlbedoMapPath, AlbedoMapPath, false, true, false);
	NormalMap = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(NormalMapPath, NormalMapPath, false, true, true);

	return true;
}

bool Material::Unload()
{
	return true;
}

void Material::InitializeBuffers()
{
	AlbedoMap->InitializeBuffer();
	NormalMap->InitializeBuffer();
}

void Material::DestroyBuffers()
{
	AlbedoMap->DestroyBuffer();
	NormalMap->DestroyBuffer();
}

void Material::Use()
{
	ShaderInstance->Use();
	AlbedoMap->Use(GL_TEXTURE0);
	NormalMap->Use(GL_TEXTURE1);
}

Material::Material()
	: Resource( HashString::NONE() )
{
}
