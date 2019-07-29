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
	IntializeComponents();
}

void SceneObjectBase::IntializeComponents()
{

}
