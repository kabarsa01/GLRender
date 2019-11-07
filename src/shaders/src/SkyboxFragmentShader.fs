#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube SkyboxMap;

void main()
{
//	float depth = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near); // linearize and scale to [0,1] for demo purposes
//	FragColor = vec4(vec3(depth), 1.0);
	FragColor = texture(SkyboxMap, TexCoord);
}
