#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdio>
#include <stdio.h>

#include "core/Engine.h"
#include "render/Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLRender", NULL, NULL);
	if (window == nullptr)
	{
		std::printf("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (! gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ))
	{
		std::printf("Failed to init GLAD");
		return -1;
	}

	glViewport(200, 200, 800, 600);

	// renderer initialization
	Engine* Engine = Engine::GetInstance();
	Engine->InitModules();
	RendererPtr renderer = Engine->GetRenderer();
	renderer->Init();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		Engine->Tick();
		//renderer->RenderFrame();

		// events and buffers swapping
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

