#version 330 core
layout(location = 0) in vec3 InPos;
layout(location = 1) in vec3 InColor;
layout(location = 2) in vec2 InUV;

out vec3 vertexColor;
out vec2 uv;

uniform mat4 transform;

void main()
{
	//gl_Position = vec4(InPos.x, InPos.y, InPos.z, 1.0);
	gl_Position = transform * vec4(InPos, 1.0);
	vertexColor = InColor;
	uv = InUV;
}