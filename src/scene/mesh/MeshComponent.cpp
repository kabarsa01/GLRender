#include "scene/mesh/MeshComponent.h"
#include <array>

// default mesh data is a quad
//namespace
//{
	float vertices[] = {
		// coords              // color             // uv coords
		 0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
//}

MeshComponent::MeshComponent(std::shared_ptr<SceneObjectBase> Parent)
	: SceneObjectComponent(Parent)
{

}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::SetMeshData(const std::vector<float>& InVerticesData, const std::vector<unsigned int>& InIndicesData)
{
	VerticesData = InVerticesData;
	IndicesData = InIndicesData;
}

std::vector<float>& MeshComponent::GetVerticesData()
{
	return VerticesData;
}

std::vector<unsigned int>& MeshComponent::GetIndicesData()
{
	return IndicesData;
}

void MeshComponent::Initialize()
{
	SceneObjectComponent::Initialize();
	// default quad mesh
	VerticesData.assign(vertices, vertices + 32);
	IndicesData.assign(indices, indices + 6);
}
