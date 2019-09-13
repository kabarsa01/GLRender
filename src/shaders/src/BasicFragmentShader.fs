#version 330 core

in vec3 world_pos;
in vec3 normal;
in vec2 uv;

uniform float spec_strength;

uniform vec3 ambient_color;
uniform vec3 light_dir;
uniform vec3 light_color;
uniform vec3 spec_color;
uniform vec3 view_pos;

uniform sampler2D albedo;
uniform sampler2D secondaryAlbedo;

out vec4 FragColor;

void main()
{
//	FragColor = vec4(vertexColor, 1.0f);
//    FragColor = mix(texture(albedo, uv), texture(secondaryAlbedo, uv), 0.25f);
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(world_pos - view_pos);
	vec3 light_dir_norm = normalize(-1.0 * light_dir);
    vec3 reflect_dir = reflect(light_dir_norm, norm);

    vec3 spec = spec_color * pow(max(dot(reflect_dir, view_dir), 0.0), 32) * spec_strength;
	vec3 light_res_color = max(dot(light_dir_norm, norm), 0.0f) * light_color;
	FragColor = vec4(vec3(texture(albedo, uv)) * (light_res_color + ambient_color + spec), 1.0);
}
