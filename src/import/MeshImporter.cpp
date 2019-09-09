#include "import/MeshImporter.h"
#include "core/ObjectBase.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

std::shared_ptr<MeshData> MeshImporter::Import(std::string InPath)
{
	Assimp::Importer LocalImporter;
	const aiScene* Scene = LocalImporter.ReadFile(InPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if ( (Scene == nullptr) || (Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || (Scene->mRootNode == nullptr) )
	{
		cout << "ASSIMP::ERROR import " << LocalImporter.GetErrorString() << endl;
		return std::shared_ptr<MeshData>();
	}

	ImportedData = ObjectBase::NewObject<MeshData>(InPath);
}

void MeshImporter::ProcessNode(aiNode * node, const aiScene * scene)
{

}
