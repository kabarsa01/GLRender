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
	CameraComp = std::shared_ptr<CameraComponent>(new CameraComponent(shared_from_this()));
	Components.push_back(std::dynamic_pointer_cast<SceneObjectComponent>(CameraComp));
}
