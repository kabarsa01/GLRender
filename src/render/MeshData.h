#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ObjectBase.h"

using namespace std;
using namespace glm;

//packing should probably be considered
//#pragma pack(push, 1)
//#pragma pack(pop)

struct Vertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
};


class MeshData : public ObjectBase
{
public:
	vector<Vertex> Vertices;
	vector<unsigned int> Indices;
};

