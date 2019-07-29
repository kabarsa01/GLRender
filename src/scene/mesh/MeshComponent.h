#pragma once

#include "scene/SceneObjectComponent.h"
#include <vector>

class MeshComponent : public SceneObjectComponent
{
public:
	MeshComponent(std::shared_ptr<SceneObjectBase> Parent);
	virtual ~MeshComponent();

	void SetMeshData(std::vector<float> InMeshData);
	std::vector<float> GetMeshData();

	virtual void Initialize() override;
protected:
	std::vector<float> MeshData;
};
