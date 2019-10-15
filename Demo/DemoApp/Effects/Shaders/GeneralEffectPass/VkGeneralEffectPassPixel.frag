#version 450

#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS_PER_TYPE 16
#define MAX_ALL_LIGHTS MAX_LIGHTS_PER_TYPE * 3

struct Material
{
    vec4 DiffuseAlbedo;
    vec3 FresnelR0;
    float Roughness;
};

struct Light {
    vec3 Strength;
    float FalloffStart; 
    vec3 Direction; 
    float FalloffEnd; 
    vec3 Position; 
    float SpotPower; 
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return clamp((falloffEnd - d) / (falloffEnd - falloffStart), 0, 1);
}

vec3 SchlickFresnel(vec3 R0, vec3 normal, vec3 lightVector){
    float cosIncidentAngle = clamp(dot(normal, lightVector), 0, 1);

    float f0 = 1.0f - cosIncidentAngle;
    vec3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}

vec3 BlinnPhong(vec3 lightStrength, vec3 lightVector, vec3 normal, vec3 toEye, Material material)
{
	//see https://github.com/d3dcoder/d3d12book to learn more about Blinn Phong
    const float m = (1.0f - material.Roughness) * 256.0f;
    vec3 halfVec = normalize(toEye + lightVector);

    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    vec3 fresnelFactor = SchlickFresnel(material.FresnelR0, halfVec, lightVector);
    vec3 specAlbedo = fresnelFactor * roughnessFactor;

    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (material.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

vec3 ComputeDirectionalLight(Light light, Material material, vec3 normal, vec3 toEye)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return vec3(0.0f, 0.0f, 0.0f);

    vec3 lightVector = -light.Direction;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    vec3 lightStrength = light.Strength * ndotl;

    return BlinnPhong(lightStrength, lightVector, normal, toEye, material);
}

vec3 ComputePointLight(Light light, Material material, vec3 position, vec3 normal, vec3 toEye)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return vec3(0.0f, 0.0f, 0.0f);

    vec3 lightVector = light.Position - position;
    float d = length(lightVector);

    if (d > light.FalloffEnd) return vec3(0.0f, 0.0f, 0.0f);

    lightVector = lightVector / d;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    vec3 lightStrength = light.Strength * ndotl;

    float att = CalcAttenuation(d, light.FalloffStart, light.FalloffEnd);

    lightStrength = lightStrength * att;

    return BlinnPhong(lightStrength, lightVector, normal, toEye, material);
}

vec3 ComputeSpotLight(Light light, Material material, vec3 position, vec3 normal, vec3 toEye)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return vec3(0.0f, 0.0f, 0.0f);

    vec3 lightVector = light.Position - position;
    float d = length(lightVector);

    if (d > light.FalloffEnd) return vec3(0.0f, 0.0f, 0.0f);

    lightVector = lightVector / d;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    vec3 lightStrength = light.Strength * ndotl;

    float att = CalcAttenuation(d, light.FalloffStart, light.FalloffEnd);
    lightStrength = lightStrength * att;

    float spotFactor = pow(max(dot(-lightVector, light.Direction), 0.0f), light.SpotPower);

    lightStrength = lightStrength * spotFactor;

    return BlinnPhong(lightStrength, lightVector, normal, toEye, material);
}

vec4 ComputeLighting(Light lights[MAX_ALL_LIGHTS], Material material, vec3 position, vec3 normal, vec3 toEye)
{
    normal = normalize(normal);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < MAX_LIGHTS_PER_TYPE; i++)
    {
        result = result + ComputeDirectionalLight(lights[0 * MAX_LIGHTS_PER_TYPE + i], material, normal, toEye);
        result = result + ComputePointLight(lights[1 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye);
        result = result + ComputeSpotLight(lights[2 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye);
    }

    return vec4(result.xyz, material.DiffuseAlbedo.a);
}

layout (set = 0, binding = 0) uniform Lights
{
    Light instance[MAX_ALL_LIGHTS];
} lights;

layout (set = 0, binding = 1) buffer Materials
{
    Material instance[];
} materials;

layout (push_constant) uniform Index
{
	float ambientLightRed;
	float ambientLightGreen;
	float ambientLightBlue;
	float ambientLightAlpha;
} index;

layout (location = 0) in vec3 viewPosition;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in flat uint instanceId;

layout (location = 0) out vec4 outColor;

void main()
{
    vec3 toEye = normalize(vec3(0.0f, 0.0f, 0.0f) - viewPosition);
    
	vec4 ambient = vec4(
		index.ambientLightRed, 
		index.ambientLightGreen, 
		index.ambientLightBlue, 
		index.ambientLightAlpha) * materials.instance[instanceId].DiffuseAlbedo;

    outColor = ComputeLighting(lights.instance, materials.instance[instanceId],
        position, normal, toEye) + ambient;

    outColor.a = materials.instance[instanceId].DiffuseAlbedo.a;
}