#include "MeshData.h"
#include "data/DataManager.h"

#include <glad/glad.h>

namespace
{
	std::vector<Vertex> QuadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions          // texCoords
	{{-1.0f,  1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{ 1.0f, -1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
	{{ 1.0f,  1.0f,  0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}
	};

	std::vector<unsigned int> QuadIndices = {
		0, 1, 2,
		0, 2, 3
	};

	std::string FullscreenQuadId = "MeshData_FullscreenQuad";
};

MeshData::MeshData(const string& InId)
	: Resource{InId}
	, VAO{ (unsigned int)-1 }
	, VBO{ (unsigned int)-1 }
	, EBO{ (unsigned int)-1 }
{

}

MeshData::MeshData(const string& InId, const std::vector<Vertex>& InVertices, const std::vector<unsigned int>& InIndices)
	: Resource{ InId }
	, VAO{ (unsigned int)-1 }
	, VBO{ (unsigned int)-1 }
	, EBO{ (unsigned int)-1 }
	, Vertices( InVertices )
	, Indices( InIndices )
{
}

MeshData::~MeshData()
{

}

void MeshData::OnDestroy()
{
	DestroyBufferObjects();
}

void MeshData::SetupBufferObjects()
{
	// VAO & VBO init
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// VAO bind
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), static_cast<void*>(&Vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), static_cast<void*>(&Indices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Bitangent)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshData::DestroyBufferObjects()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (VBO != -1) glDeleteBuffers(1, &VBO);
	if (EBO != -1) glDeleteBuffers(1, &EBO);
	if (VAO != -1) glDeleteVertexArrays(1, &VAO);
}

void MeshData::Draw()
{
	// bind VAO and draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

MeshDataPtr MeshData::FullscreenQuad()
{
	return DataManager::GetInstance()->RequestResourceByType<MeshData, const string&, const std::vector<Vertex>&, const std::vector<unsigned int>&>(
		FullscreenQuadId,
		FullscreenQuadId,
		QuadVertices,
		QuadIndices
	);
}

bool MeshData::Load()
{
	return false;
}

bool MeshData::Unload()
{
	return false;
}
