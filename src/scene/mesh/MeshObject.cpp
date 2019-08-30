#include "MeshObject.h"

MeshObject::MeshObject()
{
}


MeshObject::~MeshObject()
{
}

void MeshObject::Initialize()
{
	SceneObjectBase::Initialize();
}

std::shared_ptr<MeshComponent> MeshObject::GetMeshComponent()
{
	return MeshComp;
}

void MeshObject::IntializeComponents()
{
	SceneObjectBase::IntializeComponents();

	MeshComp = ObjectBase::NewObject<MeshComponent, std::shared_ptr<SceneObjectBase>>(derived_shared_from_this<SceneObjectBase>());
}
