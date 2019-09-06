#pragma once

#include "scene/SceneObjectComponent.h"
#include "render/MeshData.h"

#include <vector>

class MeshComponent : public SceneObjectComponent
{
public:
	MeshData MeshData;

	MeshComponent(std::shared_ptr<SceneObjectBase> Parent);
	virtual ~MeshComponent();

	void SetMeshData(const std::vector<float>& InVerticesData, const std::vector<unsigned int>& InIndicesData);
	std::vector<float>& GetVerticesData();
	std::vector<unsigned int>& GetIndicesData();

	virtual void Initialize() override;
protected:
	std::vector<float> VerticesData;
	std::vector<unsigned int> IndicesData;
};
