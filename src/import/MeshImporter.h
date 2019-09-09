#pragma once

#include <string>
#include <memory>
#include "render/MeshData.h"

#include <assimp/scene.h>

//struct aiNode;
//struct aiScene;

class MeshImporter
{
public:
	std::shared_ptr<MeshData> Import(std::string InPath);
protected:
	std::shared_ptr<MeshData> ImportedData;

	void ProcessNode(aiNode *node, const aiScene *scene);
};

