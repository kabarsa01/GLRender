#pragma once
#include "core\ObjectBase.h"
#include <memory>

class SceneObjectBase;

class SceneObjectComponent : public ObjectBase
{
public:
	SceneObjectComponent(std::shared_ptr<SceneObjectBase> InParent);
	virtual ~SceneObjectComponent();

	virtual void Initialize() override;
	std::shared_ptr<SceneObjectBase> GetParent();
protected:
	std::shared_ptr<SceneObjectBase> Parent;

	bool Register();
};


