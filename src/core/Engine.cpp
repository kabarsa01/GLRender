#include "core/Engine.h"
#include "core/TimeManager.h"

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

void Engine::Tick()
{
	TimeManager::GetInstance()->UpdateTime();

	SceneInstance->PerFrameUpdate();
	RendererInstance->RenderFrame();
}

Engine::Engine()
{
}

Engine::~Engine()
{
}


