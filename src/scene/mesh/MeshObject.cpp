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

	MeshComp = NewObject<MeshComponent, SceneObjectBase>(shared_from_this());
}
