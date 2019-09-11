#include "scene/SceneObjectComponent.h"
#include "scene/SceneObjectBase.h"


SceneObjectComponent::SceneObjectComponent(std::shared_ptr<SceneObjectBase> InParent)
	: ObjectBase()
	, Parent(InParent)
{
}

SceneObjectComponent::SceneObjectComponent()
	: ObjectBase()
{

}

SceneObjectComponent::~SceneObjectComponent()
{

}

void SceneObjectComponent::Initialize()
{
	if (Parent.get() != nullptr)
	{
		Parent->RegisterComponent(get_shared_from_this<SceneObjectComponent>());
	}
//	Scene
}

std::shared_ptr<SceneObjectBase> SceneObjectComponent::GetParent()
{
	return Parent;
}

bool SceneObjectComponent::Register()
{
	return true;
}

