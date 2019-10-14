#version 330 core

in VS_OUT
{
	vec3 world_pos;
	vec3 normal;
	vec2 uv;
	mat3 TBN;
} fs_in;

uniform float spec_strength;

uniform vec3 ambient_color;
uniform vec3 light_dir;
uniform vec3 light_color;
uniform vec3 spec_color;
uniform vec3 view_pos;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;

out vec4 FragColor;


////////////////////////////////////////////////////////////////////////
float near = 0.01; 
float far  = 30.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
////////////////////////////////////////////////////////////////////////

vec3 CalculateSpec(vec3 InViewDir, vec3 InLightDir, vec3 InNormal, vec3 InSpecColor, float InSpecStrength)
{
	vec3 Intermediate = normalize(InViewDir + InLightDir);
	return InSpecColor * pow(max(dot(Intermediate, InNormal), 0.0), 32) * InSpecStrength;
}

vec3 CalculateNormal()
{
	vec3 normalVec = texture(NormalMap, fs_in.uv).rgb;
	normalVec = normalize(normalVec * 2.0 - 1.0);
	normalVec = normalize(fs_in.TBN * normalVec);
	return normalVec;
}

void main()
{
    vec3 norm = CalculateNormal();//normalize(fs_in.normal);
    vec3 view_dir = normalize(fs_in.world_pos - view_pos);
	vec3 light_dir_norm = normalize(-1.0 * light_dir);
    vec3 reflect_dir = reflect(light_dir_norm, norm);

    //vec3 spec = spec_color * pow(max(dot(reflect_dir, view_dir), 0.0), 32) * spec_strength;
	vec3 spec = CalculateSpec(light_dir_norm, -1.0 * view_dir, norm, spec_color, spec_strength);
	vec3 light_res_color = max(dot(light_dir_norm, norm), 0.0f) * light_color;
	FragColor = vec4(vec3(texture(AlbedoMap, fs_in.uv)) * (light_res_color + ambient_color + spec), 1.0);
	FragColor.rgb = pow(FragColor.rgb, vec3(1/2.2));
	//FragColor.rgb = norm;

	/////////////////////
//	float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
//    FragColor = vec4(vec3(depth), 1.0);
}
