#include "scene/Scene.h"
#include "scene/SceneObjectBase.h"
#include "scene/SceneObjectComponent.h"

Scene::Scene()
	: ObjectBase()
{
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
	ObjectBase::Initialize();
}

void Scene::RegisterSceneObject(SceneObjectBasePtr InSceneObject)
{
}

void Scene::RemoveSceneObject(SceneObjectBasePtr InSceneObject)
{
}

void Scene::RegisterSceneObjectComponent(SceneObjectComponentPtr InSceneObjectComponent)
{

}
