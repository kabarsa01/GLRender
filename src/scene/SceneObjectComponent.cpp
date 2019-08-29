#include "scene/SceneObjectComponent.h"
#include "scene/SceneObjectBase.h"


SceneObjectComponent::SceneObjectComponent(std::shared_ptr<SceneObjectBase> InParent)
	: Parent(InParent)
{
}

SceneObjectComponent::~SceneObjectComponent()
{

}

void SceneObjectComponent::Initialize()
{
	if (Parent.get() != nullptr)
	{
		Parent->RegisterComponent(derived_shared_from_this<SceneObjectComponent>());
	}
}

std::shared_ptr<SceneObjectBase> SceneObjectComponent::GetParent()
{
	return Parent;
}

bool SceneObjectComponent::Register()
{
	return true;
}

