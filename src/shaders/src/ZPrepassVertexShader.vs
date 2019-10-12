#version 330 core
layout(location = 0) in vec3 InPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 world_pos = vec3(model * vec4(InPos, 1.0));
	gl_Position = projection * view * vec4(world_pos, 1.0);
//	gl_Position = projection * view * model * vec4(InPos, 1.0);
}