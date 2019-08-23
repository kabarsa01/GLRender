#pragma once

#include "scene/Transform.h"
#include <vector>
#include <memory>
#include "core/ObjectBase.h"

class SceneObjectComponent;

class SceneObjectBase : public ObjectBase, public std::enable_shared_from_this<SceneObjectBase>
{
public:
	// fields
	Transform Transform;

	// methods
	SceneObjectBase();
	virtual ~SceneObjectBase();

	virtual void Initialize() override;
	bool RegisterComponent(std::shared_ptr<SceneObjectComponent> InComponent);
protected:
	// components container
	std::vector<std::shared_ptr<SceneObjectComponent>> Components;

	virtual void IntializeComponents();
};

