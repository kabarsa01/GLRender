#include "scene/CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "scene/SceneObjectBase.h"
#include "scene/SceneObjectComponent.h"

CameraComponent::CameraComponent(std::shared_ptr<SceneObjectBase> InParent)
	: SceneObjectComponent(InParent)
	, FOV(60.0f)
	, AspectRatio(16.0f / 9.0f)
	, NearPlane(0.1f)
	, FarPlane(100.0f)
{

}

CameraComponent::~CameraComponent()
{
}

glm::mat4 CameraComponent::CalculateViewMatrix() const
{
	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, Parent->Transform.GetLocation() * -1.0f);
	return view;
}

glm::mat4 CameraComponent::CalculateProjectionMatrix() const
{
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(FOV), AspectRatio, NearPlane, FarPlane);
	return projection;
}
