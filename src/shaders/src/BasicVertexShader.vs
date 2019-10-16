#version 330 core

layout(location = 0) in vec3 InPos;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InUV;
layout(location = 3) in vec3 InTangent;
layout(location = 4) in vec3 InBitangent;

invariant gl_Position;

out VS_OUT
{
	vec4 light_pos;
	vec3 world_pos;
	vec3 normal;
	vec2 uv;
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_view;

void main()
{
	gl_Position = projection * view * model * vec4(InPos, 1.0);
	vs_out.world_pos = vec3(model * vec4(InPos, 1.0));
	mat3 normal_mat = mat3(transpose(inverse(model)));
	vs_out.normal = normal_mat * InNormal;
	vs_out.uv = InUV;

	vec3 T = normalize(normal_mat * InTangent);
	vec3 B = normalize(normal_mat * InBitangent);
	vec3 N = normalize(normal_mat * InNormal);
	vs_out.TBN = mat3(T, B, N);

	vs_out.light_pos =  light_view * vec4(vs_out.world_pos, 1.0);
}