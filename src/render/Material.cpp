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
	AlbedoMap = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(AlbedoMapPath, AlbedoMapPath, false, true, false);
	NormalMap = DM->RequestResourceByType<Texture, const std::string&, bool, bool, bool>(NormalMapPath, NormalMapPath, false, true, true);

	return true;
}

bool Material::Unload()
{
	return true;
}
