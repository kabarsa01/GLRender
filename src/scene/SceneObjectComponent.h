#pragma once
#include "core\ObjectBase.h"
#include <memory>

class SceneObjectBase;

class SceneObjectComponent : public ObjectBase, public std::enable_shared_from_this<SceneObjectComponent>
{
public:
	SceneObjectComponent(std::shared_ptr<SceneObjectBase> InParent);
	virtual ~SceneObjectComponent();

	std::shared_ptr<SceneObjectBase> GetParent();
protected:
	std::shared_ptr<SceneObjectBase> Parent;

	bool Register();
};