#pragma once

#include "core/ObjectBase.h"
#include "scene/Scene.h"
#include "render/Renderer.h"

class Engine
{
public:
	static Engine* GetInstance();
	static ScenePtr GetSceneInstance();
	static RendererPtr GetRendererInstance();

	void InitModules();

	ScenePtr GetScene();
	RendererPtr GetRenderer();

	void Tick();
protected:
	ScenePtr SceneInstance;
	RendererPtr RendererInstance;
private:
	static Engine* StaticInstance;

	Engine();
	virtual ~Engine();
};
