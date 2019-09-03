#pragma once

#include <map>
#include <vector>
#include <memory>

#include "core/ObjectBase.h"

class SceneObjectBase;

class Scene : public ObjectBase
{
public:
	Scene();
	virtual ~Scene();
protected:
	std::vector<std::shared_ptr<SceneObjectBase>> SceneObjects;
};