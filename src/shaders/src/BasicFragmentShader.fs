#version 330 core

in VS_OUT
{
	vec4 light_pos;
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
uniform sampler2D MetallnessMap;
uniform sampler2D RoughnessMap;
uniform sampler2D AOMap;
uniform sampler2D ShadowMap;

out vec4 FragColor;

const float PI = 3.14159265359;
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
    // blinn-phong intermediate vector and spec value calculation
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

// shadow calculation
float CalculateShadow(vec4 LightSpacePos, vec3 LightDir, vec3 Normal)
{
    vec3 ProjectedPos = LightSpacePos.xyz / LightSpacePos.w;
    ProjectedPos = ProjectedPos * 0.5 + 0.5;
    if (ProjectedPos.z > 1.0)
    {
        return 0.0;
    }
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    float bias = max(0.02 * (1.0 - dot(Normal, LightDir)), 0.005);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ShadowMap, ProjectedPos.xy + vec2(x, y) * texelSize).r; 
            shadow += ( (ProjectedPos.z - bias) > pcfDepth ) ? 1.0 : 0.0;        
        }    
    }
    return shadow /= 9.0;
    //float ShadowDepth = texture(ShadowMap, ProjectedPos.xy).r;
    //float bias = max(0.02 * (1.0 - dot(Normal, LightDir)), 0.005);
    //return ( (ProjectedPos.z - bias) > ShadowDepth ) ? 1.0 : 0.0;
}

vec3 FresnelSchlick(float CosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - CosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float Roughness)
{
    float a      = Roughness*Roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float Roughness)
{
    float r = (Roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float Roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, Roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, Roughness);
	
    return ggx1 * ggx2;
}

void main()
{
    vec3 Albedo = texture(AlbedoMap, fs_in.uv).rgb;
    float Metallic = 0.0;//texture(MetallnessMap, fs_in.uv).r;
    float Roughness = texture(RoughnessMap, fs_in.uv).r;
    float AO = 1.0;//texture(AOMap, fs_in.uv).r;

    vec3 N = CalculateNormal();//normalize(fs_in.normal);
    vec3 V = normalize(view_pos - fs_in.world_pos);
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 1; ++i)
    {
        vec3 L = normalize(-1.0 * light_dir);
        vec3 H = normalize(V + L);

        float Attenuation = 1.0;
        vec3 Radiance = light_color * Attenuation;

        vec3 F0 = vec3(0.04); 
        F0 = mix(F0, Albedo, Metallic);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
        float NDF = DistributionGGX(N, H, Roughness);  
        float G = GeometrySmith(N, V, L, Roughness);

        vec3 Numerator = NDF * G * F;
        float Denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 Specular = Numerator / max(Denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metallic;
  
        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * Albedo / PI + Specular) * Radiance * NdotL;
    }

    vec3 Ambient = vec3(0.03) * Albedo * AO;
    vec3 Color = Ambient + Lo;
	
// HDR tonemapping
//    Color = Color / (Color + vec3(1.0));
    Color = pow(Color, vec3(1.0/2.2));  
   
    FragColor = vec4(Color, 1.0);
 //   vec3 norm = CalculateNormal();//normalize(fs_in.normal);
 //   vec3 view_dir = normalize(fs_in.world_pos - view_pos);
	//vec3 light_dir_norm = normalize(-1.0 * light_dir);
 //   vec3 reflect_dir = reflect(light_dir_norm, norm);

 //   //vec3 spec = spec_color * pow(max(dot(reflect_dir, view_dir), 0.0), 32) * spec_strength;
	//vec3 spec = CalculateSpec(light_dir_norm, -1.0 * view_dir, norm, spec_color, spec_strength);
	//vec3 light_res_color = max(dot(light_dir_norm, norm), 0.0f) * light_color;
 //   float shadow = CalculateShadow(fs_in.light_pos, light_dir_norm, norm);
	//FragColor = vec4(vec3(texture(AlbedoMap, fs_in.uv)) * ((light_res_color + spec) * (1.0 - shadow) + ambient_color), 1.0);
	//FragColor.rgb = pow(FragColor.rgb, vec3(1/2.2));
	//FragColor.rgb = norm;

	/////////////////////
//	float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
//    FragColor = vec4(vec3(depth), 1.0);
}
