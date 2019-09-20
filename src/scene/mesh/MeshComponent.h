#pragma once

#include "scene/SceneObjectComponent.h"
#include "render/MeshData.h"

#include <vector>

class MeshComponent : public SceneObjectComponent
{
public:
	std::shared_ptr<MeshData> MeshDataPtr;

	MeshComponent(std::shared_ptr<SceneObjectBase> Parent);
	virtual ~MeshComponent();

	void SetMeshData(std::shared_ptr<MeshData> InMeshData);
	virtual void Initialize() override;
protected:
};

typedef std::shared_ptr<MeshComponent> MeshComponentPtr;

