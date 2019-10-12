#version 330 core

out vec4 FragColor;

float near = 0.15; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
//	float depth = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near); // linearize and scale to [0,1] for demo purposes
//	FragColor = vec4(vec3(depth), 1.0);
	FragColor = vec4(vec3(0.5), 1.0);
}
