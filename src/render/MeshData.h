#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "data/Resource.h"

using namespace std;
using namespace glm;

//packing should probably be considered, maybe in the future someday
//#pragma pack(push, 1)
//#pragma pack(pop)

struct Vertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
	vec3 Tangent;
	vec3 Bitangent;
};


class MeshData : public Resource
{
public:
	vector<Vertex> Vertices;
	vector<unsigned int> Indices;
	// gl objects for mesh data
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int EBO = -1;

	MeshData(string InId);
	virtual ~MeshData();

	// Inherited via Resource
	virtual bool Load() override;
	virtual bool Unload() override;

	void SetupBufferObjects();
	void DestroyBufferObjects();
	void Draw();
private:
	MeshData() : Resource(HashString::NONE()) {}
};

typedef std::shared_ptr<MeshData> MeshDataPtr;

