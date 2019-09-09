#version 330 core

in vec3 vertexColor;
in vec2 uv;

uniform sampler2D albedo;
uniform sampler2D secondaryAlbedo;

out vec4 FragColor;

void main()
{
//	FragColor = vec4(vertexColor, 1.0f);
//    FragColor = mix(texture(albedo, uv), texture(secondaryAlbedo, uv), 0.25f);
	FragColor = texture(albedo, uv);
}
