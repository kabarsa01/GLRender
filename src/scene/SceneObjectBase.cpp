#include "scene/SceneObjectBase.h"
#include "scene/SceneObjectComponent.h"
#include "scene/Scene.h"
#include "core/Engine.h"

SceneObjectBase::SceneObjectBase()
{
}

SceneObjectBase::~SceneObjectBase()
{
}

void SceneObjectBase::Initialize()
{
	ObjectBase::Initialize();

	ScenePtr Scene = Engine::GetInstance()->GetScene();
	Scene->RegisterSceneObject(get_shared_from_this<SceneObjectBase>());

	IntializeComponents();
}

bool SceneObjectBase::RegisterComponent(std::shared_ptr<SceneObjectComponent> InComponent)
{
	Components.push_back(InComponent);

	ScenePtr Scene = Engine::GetInstance()->GetScene();
	Scene->RegisterSceneObjectComponent(InComponent);

	return true;
}

void SceneObjectBase::Tick(float DeltaTime)
{
}

void SceneObjectBase::TickComponents(float DeltaTime)
{
	for (SceneObjectComponentPtr Comp : Components)
	{
		if (Comp->IsTickEnabled)
		{
			Comp->TickComponent(DeltaTime);
		}
	}
}

std::vector<SceneObjectComponentPtr> SceneObjectBase::GetComponents() const
{
	return Components;
}

void SceneObjectBase::Destroy()
{
	ScenePtr Scene = Engine::GetInstance()->GetScene();
	ObjectBase::Destroy();
}

void SceneObjectBase::IntializeComponents()
{

}
