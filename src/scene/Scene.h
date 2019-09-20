#pragma once

#include <map>
#include <set>
#include <memory>

#include "core/ObjectBase.h"

class SceneObjectBase;
typedef std::shared_ptr<SceneObjectBase> SceneObjectBasePtr;

class Scene : public ObjectBase
{
public:
	Scene();
	virtual ~Scene();
	
	void RegisterSceneObject(SceneObjectBasePtr InSceneObject);
	void RemoveSceneObject(SceneObjectBasePtr InSceneObject);
protected:
	std::map<size_t, std::set<SceneObjectBasePtr>> SceneObjects;
};


typedef std::shared_ptr<Scene> ScenePtr;

