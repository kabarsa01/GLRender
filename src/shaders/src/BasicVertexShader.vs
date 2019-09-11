#version 330 core
layout(location = 0) in vec3 InPos;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InUV;

out vec3 world_pos;
out vec3 normal;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = vec4(InPos.x, InPos.y, InPos.z, 1.0);
	world_pos = vec3(model * vec4(InPos, 1.0));
	gl_Position = projection * view * vec4(world_pos, 1.0);
	normal = mat3(transpose(inverse(model))) * InNormal;
	uv = InUV;
}