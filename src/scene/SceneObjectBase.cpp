#include "scene/SceneObjectBase.h"
#include "scene/SceneObjectComponent.h"

SceneObjectBase::SceneObjectBase()
{
}

SceneObjectBase::~SceneObjectBase()
{
}

void SceneObjectBase::Initialize()
{
	ObjectBase::Initialize();
	IntializeComponents();
}

bool SceneObjectBase::RegisterComponent(std::shared_ptr<SceneObjectComponent> InComponent)
{
	Components.push_back(InComponent);
	return true;
}

void SceneObjectBase::IntializeComponents()
{

}
