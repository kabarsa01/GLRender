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
	ShaderInstance = DM->RequestResourceByType<Shader>(VertexShaderPath + FragmentShaderPath, VertexShaderPath, FragmentShaderPath);
	AlbedoMap = DM->RequestResourceByType<Texture>(AlbedoMapPath, AlbedoMapPath);
	NormalMap = DM->RequestResourceByType<Texture>(NormalMapPath, NormalMapPath);

	return true;
}

bool Material::Unload()
{
	return true;
}
