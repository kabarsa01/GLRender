#include "scene/SceneObjectComponent.h"
#include "scene/SceneObjectBase.h"


SceneObjectComponent::SceneObjectComponent(std::shared_ptr<SceneObjectBase> InParent)
	: Parent(InParent)
{
	if (Parent.get() != nullptr)
	{
//		Parent->Components.push_back(shared_from_this());
	}
}

SceneObjectComponent::~SceneObjectComponent()
{

}

std::shared_ptr<SceneObjectBase> SceneObjectComponent::GetParent()
{
	return Parent;
}

bool SceneObjectComponent::Register()
{
	return true;
}

