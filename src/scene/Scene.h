#pragma once

#include <map>
#include <set>
#include <memory>

#include "core/ObjectBase.h"
#include "core/Class.h"
#include "common/HashString.h"

class SceneObjectBase;
typedef std::shared_ptr<SceneObjectBase> SceneObjectBasePtr;
class SceneObjectComponent;
typedef std::shared_ptr<SceneObjectComponent> SceneObjectComponentPtr;

class Scene : public ObjectBase
{
public:
	Scene();
	virtual ~Scene();

	virtual void Initialize() override;
	
	void RegisterSceneObject(SceneObjectBasePtr InSceneObject);
	void RemoveSceneObject(SceneObjectBasePtr InSceneObject);

	void RegisterSceneObjectComponent(SceneObjectComponentPtr InSceneObjectComponent);
	void RemoveSceneObjectComponent(SceneObjectComponentPtr InSceneObjectComponent);

	void PerFrameUpdate();

	template<class T>
	std::set<SceneObjectComponentPtr> GetSceneComponents();
protected:
	std::set<SceneObjectBasePtr> SceneObjectsSet;
	std::map<HashString, std::set<SceneObjectBasePtr>> SceneObjectsMap;
	std::map<HashString, std::set<SceneObjectComponentPtr>> SceneObjectComponents;
};

typedef std::shared_ptr<Scene> ScenePtr;

//=============================================================================================================
// TEMPLATED DEFINITIONS
//=============================================================================================================

template<class T>
inline std::set<SceneObjectComponentPtr> Scene::GetSceneComponents()
{
	HashString Key = Class::Get<T>().GetName();
	return SceneObjectComponents[Key];
}
