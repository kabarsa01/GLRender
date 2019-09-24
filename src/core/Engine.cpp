#include "core/Engine.h"

Engine* Engine::StaticInstance = new Engine();

Engine * Engine::GetInstance()
{
	return StaticInstance;
}

void Engine::InitModules()
{
	SceneInstance = ObjectBase::NewObject<Scene>();
	RendererInstance = ObjectBase::NewObject<Renderer>();
}

ScenePtr Engine::GetScene()
{
	return SceneInstance;
}

RendererPtr Engine::GetRenderer()
{
	return RendererInstance;
}

Engine::Engine()
{
}

Engine::~Engine()
{
}


