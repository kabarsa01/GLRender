#include "MeshData.h"

#include <glad/glad.h>

MeshData::MeshData(string InId)
	: Resource{InId}
	, VAO{ (unsigned int)-1 }
	, VBO{ (unsigned int)-1 }
	, EBO{ (unsigned int)-1 }
{

}

MeshData::~MeshData()
{

}

void MeshData::SetupBufferObjects()
{
	// VAO & VBO init
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// VAO bind
	glBindVertexArray(VAO);

	//std::vector<float>& MeshData = MeshObj->GetMeshComponent()->GetVerticesData();
	//std::vector<unsigned int>& IndicesData = MeshObj->GetMeshComponent()->GetIndicesData();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), static_cast<void*>(&Vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), static_cast<void*>(&Indices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshData::Draw()
{
	// bind VAO and draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
