#version 330 core

in vec3 world_pos;
in vec3 normal;
in vec2 uv;

uniform vec3 ambient_color;
uniform vec3 light_dir;
uniform vec3 light_color;

uniform sampler2D albedo;
uniform sampler2D secondaryAlbedo;

out vec4 FragColor;

void main()
{
//	FragColor = vec4(vertexColor, 1.0f);
//    FragColor = mix(texture(albedo, uv), texture(secondaryAlbedo, uv), 0.25f);
	vec3 light_dir_norm = normalize(-1.0 * light_dir);
	vec3 light_res_color = max(dot(light_dir_norm, normalize(normal)), 0.0f) * light_color;
	FragColor = vec4(vec3(texture(albedo, uv)) * (light_res_color + ambient_color), 1.0);
}
