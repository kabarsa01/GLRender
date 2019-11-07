#version 330 core

layout(location = 0) in vec3 InPos;

invariant gl_Position;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = InPos;
	gl_Position = projection * view * vec4(InPos, 1.0);
}