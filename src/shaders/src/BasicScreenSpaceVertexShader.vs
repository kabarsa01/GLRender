#version 330 core
layout(location = 0) in vec3 InPos;
layout(location = 2) in vec2 InUV;

out vec2 uv;

void main()
{
	gl_Position = vec4(InPos.xy, 0.0, 1.0);
	uv = InUV;
}