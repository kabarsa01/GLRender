#include "CameraObject.h"



CameraObject::CameraObject()
{
}


CameraObject::~CameraObject()
{
}

std::shared_ptr<CameraComponent> CameraObject::GetCameraComponent()
{
	return CameraComp;
}

void CameraObject::IntializeComponents()
{
	CameraComp = ObjectBase::NewObject<CameraComponent, std::shared_ptr<SceneObjectBase>>(derived_shared_from_this<SceneObjectBase>());
}
