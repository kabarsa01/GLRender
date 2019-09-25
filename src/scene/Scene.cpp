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
	SceneObjects[InSceneObject->GetClass().GetName()].insert(InSceneObject);
}

void Scene::RemoveSceneObject(SceneObjectBasePtr InSceneObject)
{
	SceneObjects[InSceneObject->GetClass().GetName()].erase(InSceneObject);
}

void Scene::RegisterSceneObjectComponent(SceneObjectComponentPtr InSceneObjectComponent)
{
	SceneObjectComponents[InSceneObjectComponent->GetClass().GetName()].insert(InSceneObjectComponent);
}

void Scene::RemoveSceneObjectComponent(SceneObjectComponentPtr InSceneObjectComponent)
{
	SceneObjectComponents[InSceneObjectComponent->GetClass().GetName()].erase(InSceneObjectComponent);
}
