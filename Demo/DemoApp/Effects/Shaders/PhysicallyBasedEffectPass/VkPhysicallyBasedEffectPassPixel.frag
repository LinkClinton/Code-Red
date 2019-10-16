#version 450

#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS_PER_TYPE 16
#define MAX_ALL_LIGHTS MAX_LIGHTS_PER_TYPE * 3

#define MATERIAL_BUFFER 0
#define MATERIAL_TEXTURE 1

#define PI 3.14159265359

struct Material
{
    vec4 DiffuseAlbedo;
	float  Metallic;
	float  Roughness;
	float  AmbientOcclusion;
    float  Unused;
};

struct Light {
    vec3 Strength;
    float FalloffStart; 
    vec3 Direction; 
    float FalloffEnd; 
    vec3 Position; 
    float SpotPower; 
};

struct Transform3D
{
    mat4 NormalTransform;
    mat4 Projection;
    mat4 Transform;
    mat4 View;
    vec4 EyePosition;
};

vec3 mix0(vec3 x, vec3 y, vec3 a)
{
    return x * (1.0 - a) + y * a;
}

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return clamp((falloffEnd - d) / (falloffEnd - falloffStart), 0, 1);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 normal, vec3 halfVector, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float normalDotHalf = max(dot(normal, halfVector), 0.0);
    float normalDotHalf2 = normalDotHalf * normalDotHalf;

    float numerator = a2;
    float denominator = (normalDotHalf2 * (a2 - 1.0) + 1.0);

    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float GeometrySchlickGGX(float normalDot, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float numerator = normalDot;
    float denominator = normalDot * (1.0 - k) + k;

    return numerator / denominator;
}

float GeometrySmith(vec3 normal, vec3 toEye, vec3 lightVector, float roughness)
{
    float normalDotEye = max(dot(normal, toEye), 0.0);
    float normalDotLight = max(dot(normal, lightVector), 0.0);
    float ggx2 = GeometrySchlickGGX(normalDotEye, roughness);
    float ggx1 = GeometrySchlickGGX(normalDotLight, roughness);

    return ggx1 * ggx2;
}

vec3 CookTorranceBRDF(Material material, vec3 radiance, vec3 lightVector, vec3 normal, vec3 toEye, vec3 F0)
{
	//see https://github.com/JoeyDeVries/LearnOpenGL to learn more about PBR and BRDF
    vec3 halfVector = normalize(toEye + lightVector);

    float  NDF = DistributionGGX(normal, halfVector, material.Roughness);
    float  G = GeometrySmith(normal, toEye, lightVector, material.Roughness);
    vec3 F = FresnelSchlick(max(dot(halfVector, toEye), 0.0), F0);

    vec3 kS = F;
    vec3 kD = 1.0f - kS;

    kD = kD * (1.0 - material.Metallic);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, toEye), 0.0) * max(dot(normal, lightVector), 0.0) + 0.001;

    return (kD * material.DiffuseAlbedo.xyz / PI + numerator / denominator) * radiance;
}

vec3 ComputeDirectionalLight(Light light, Material material, vec3 normal, vec3 toEye, vec3 F0)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return vec3(0.0f, 0.0f, 0.0f);

    vec3 lightVector = -light.Direction;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    vec3 lightStrength = light.Strength * ndotl;

    return CookTorranceBRDF(material, lightStrength, lightVector, normal, toEye, F0);
}

vec3 ComputePointLight(Light light, Material material, vec3 position, vec3 normal, vec3 toEye, vec3 F0)
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

    return CookTorranceBRDF(material, lightStrength, lightVector, normal, toEye, F0);
}

vec3 ComputeSpotLight(Light light, Material material, vec3 position, vec3 normal, vec3 toEye, vec3 F0)
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

    return CookTorranceBRDF(material, lightStrength, lightVector, normal, toEye, F0);
}

vec4 ComputeLighting(Light lights[MAX_ALL_LIGHTS], Material material, vec3 position, vec3 normal, vec3 toEye)
{
    normal = normalize(normal);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    vec3 F0 = vec3(0.04);

    F0 = mix0(F0, material.DiffuseAlbedo.xyz, vec3(material.Metallic));

    for (int i = 0; i < MAX_LIGHTS_PER_TYPE; i++)
    {
        result = result + ComputeDirectionalLight(lights[0 * MAX_LIGHTS_PER_TYPE + i], material, normal, toEye, F0);
        result = result + ComputePointLight(lights[1 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye, F0);
        result = result + ComputeSpotLight(lights[2 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye, F0);
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

layout (set = 0, binding = 2) buffer Transform
{
    Transform3D instance[];
} transforms;

layout (push_constant) uniform Index
{
	float ambientLightRed;
	float ambientLightGreen;
	float ambientLightBlue;
	float ambientLightAlpha;
    uint  materialType;
} index;

layout (location = 0) in vec3 viewPosition;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoord;
layout (location = 4) in vec3 tangent;
layout (location = 5) in flat uint instanceId;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 3) uniform texture2D diffuseAlbedoTexture;
layout (set = 0, binding = 4) uniform texture2D metallicTexture;
layout (set = 0, binding = 5) uniform texture2D normalTexture;
layout (set = 0, binding = 6) uniform texture2D roughnessTexture;
layout (set = 0, binding = 7) uniform texture2D ambientOcclusionTexture;

layout (set = 0, binding = 8) uniform sampler materialSampler;

vec3 getNormalFromTexture(vec3 normal, vec2 texcoord, vec3 tangent)
{
    if (index.materialType == MATERIAL_BUFFER) return normal;

    vec3 tangentNormal = texture(sampler2D(normalTexture, materialSampler), texcoord).xyz * 2.0 - 1.0;
    
    vec3 N = normalize(normal);
    vec3 T = normalize(tangent - dot(tangent, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3 toEye = normalize(transforms.instance[instanceId].EyePosition.xyz - position);
	
	Material material;
	
	if (index.materialType == MATERIAL_BUFFER)
		material = materials.instance[instanceId];
	else
	{
		material.DiffuseAlbedo = pow(texture(sampler2D(diffuseAlbedoTexture, materialSampler), texcoord), vec4(2.2));
        material.Metallic = texture(sampler2D(metallicTexture, materialSampler), texcoord).r;
        material.Roughness = texture(sampler2D(roughnessTexture, materialSampler), texcoord).r;
        material.AmbientOcclusion = texture(sampler2D(ambientOcclusionTexture, materialSampler), texcoord).r;
	}

	vec4 ambient = vec4(
		index.ambientLightRed, 
		index.ambientLightGreen, 
		index.ambientLightBlue, 
		index.ambientLightAlpha) * material.DiffuseAlbedo * material.AmbientOcclusion;

    vec4 color = ComputeLighting(lights.instance, material,
        position, getNormalFromTexture(normal, texcoord, tangent), toEye) + ambient;

    color = color / (color + vec4(1.0f));
    color = pow(color, vec4(1.0 / 2.2));

	outColor = vec4(color.xyz, material.DiffuseAlbedo.a);
}