#pragma once

#include "scene/SceneObjectBase.h"
#include "scene/mesh/MeshComponent.h"
#include <memory>

class MeshObject : public SceneObjectBase
{
public:
	MeshObject();
	virtual ~MeshObject();

	virtual void Initialize() override;
protected:
	std::shared_ptr<MeshComponent> MeshComp;
};
