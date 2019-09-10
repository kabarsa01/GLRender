#pragma once

#include "scene/SceneObjectComponent.h"

class LightComponent : public SceneObjectComponent
{
public:
	LightComponent(std::shared_ptr<SceneObjectBase> InParent);
	virtual ~LightComponent();
};
