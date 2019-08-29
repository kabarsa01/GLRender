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
	CameraComp = std::shared_ptr<CameraComponent>(new CameraComponent(derived_shared_from_this<SceneObjectBase>()));
}
