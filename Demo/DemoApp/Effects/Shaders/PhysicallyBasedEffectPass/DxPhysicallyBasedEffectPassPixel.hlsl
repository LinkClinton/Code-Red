#pragma pack_matrix(row_major)

#define MAX_LIGHTS_PER_TYPE 16
#define MAX_ALL_LIGHTS MAX_LIGHTS_PER_TYPE * 3

#define MATERIAL_BUFFER 0
#define MATERIAL_TEXTURE 1

#define PI 3.14159265359

struct Material
{
    float4 DiffuseAlbedo;
	float  Metallic;
	float  Roughness;
	float  AmbientOcclusion;
};

struct Light {
    float3 Strength;
    float FalloffStart; 
    float3 Direction; 
    float FalloffEnd; 
    float3 Position; 
    float SpotPower; 
};

struct Transform3D
{
	matrix NormalTransform;
	matrix Projection;
	matrix Transform;
	matrix View;
	float4 EyePosition;
};


float3 mix(float3 x, float3 y, float3 a)
{
    return x * (1.0 - a) + y * a;
}

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 normal, float3 halfVector, float roughness)
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

float GeometrySmith(float3 normal, float3 toEye, float3 lightVector, float roughness)
{
    float normalDotEye = max(dot(normal, toEye), 0.0);
    float normalDotLight = max(dot(normal, lightVector), 0.0);
    float ggx2 = GeometrySchlickGGX(normalDotEye, roughness);
    float ggx1 = GeometrySchlickGGX(normalDotLight, roughness);

    return ggx1 * ggx2;
}

float3 CookTorranceBRDF(Material material, float3 radiance, float3 lightVector, float3 normal, float3 toEye, float3 F0)
{
	//see https://github.com/JoeyDeVries/LearnOpenGL to learn more about PBR and BRDF
    float3 halfVector = normalize(toEye + lightVector);

    float  NDF = DistributionGGX(normal, halfVector, material.Roughness);
    float  G = GeometrySmith(normal, toEye, lightVector, material.Roughness);
    float3 F = FresnelSchlick(max(dot(halfVector, toEye), 0.0), F0);

    float3 kS = F;
    float3 kD = 1.0f - kS;

    kD = kD * (1.0 - material.Metallic);

    float3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, toEye), 0.0) * max(dot(normal, lightVector), 0.0);

    return (kD * material.DiffuseAlbedo.xyz / PI + numerator / max(denominator, 0.001)) * radiance;
}

float3 ComputeDirectionalLight(Light light, Material material, float3 normal, float3 toEye, float3 F0)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = -light.Direction;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    float3 lightStrength = light.Strength * ndotl;

    return CookTorranceBRDF(material, lightStrength, lightVector, normal, toEye, F0);
}

float3 ComputePointLight(Light light, Material material, float3 position, float3 normal, float3 toEye, float3 F0)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = light.Position - position;
    float d = length(lightVector);

    if (d > light.FalloffEnd) return float3(0.0f, 0.0f, 0.0f);

    lightVector = lightVector / d;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    float3 lightStrength = light.Strength * ndotl;

    float att = CalcAttenuation(d, light.FalloffStart, light.FalloffEnd);

    lightStrength = lightStrength * att;

    return CookTorranceBRDF(material, lightStrength, lightVector, normal, toEye, F0);
}

float3 ComputeSpotLight(Light light, Material material, float3 position, float3 normal, float3 toEye, float3 F0)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = light.Position - position;
    float d = length(lightVector);

    if (d > light.FalloffEnd) return float3(0.0f, 0.0f, 0.0f);

    lightVector = lightVector / d;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    float3 lightStrength = light.Strength * ndotl;

    float att = CalcAttenuation(d, light.FalloffStart, light.FalloffEnd);
    lightStrength = lightStrength * att;

    float spotFactor = pow(max(dot(-lightVector, light.Direction), 0.0f), light.SpotPower);

    lightStrength = lightStrength * spotFactor;

    return CookTorranceBRDF(material, lightStrength, lightVector, normal, toEye, F0);
}

float4 ComputeLighting(Light lights[MAX_ALL_LIGHTS], Material material, float3 position, float3 normal, float3 toEye)
{
    normal = normalize(normal);

    float3 result = float3(0.0f, 0.0f, 0.0f);
    float3 F0 = 0.04;

    F0 = mix(F0, material.DiffuseAlbedo.xyz, material.Metallic);

    for (int i = 0; i < MAX_LIGHTS_PER_TYPE; i++)
    {
        result = result + ComputeDirectionalLight(lights[0 * MAX_LIGHTS_PER_TYPE + i], material, normal, toEye, F0);
        result = result + ComputePointLight(lights[1 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye, F0);
        result = result + ComputeSpotLight(lights[2 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye, F0);
    }

    return float4(result.xyz, material.DiffuseAlbedo.a);
}

struct Lights
{
    Light instance[MAX_ALL_LIGHTS];
};

struct Index
{
	float ambientLightRed;
	float ambientLightGreen;
	float ambientLightBlue;
	float ambientLightAlpha;
	uint  materialType;
};

StructuredBuffer<Transform3D> transforms : register(t2, space0);
StructuredBuffer<Material> materials : register(t1, space0);

ConstantBuffer<Lights> lights : register(b0, space0);
ConstantBuffer<Index> index : register(b9, space0);

Texture2D diffuseAlbedoTexture : register(t3, space0);
Texture2D metallicTexture : register(t4, space0);
Texture2D normalTexture : register(t5, space0);
Texture2D roughnessTexture : register(t6, space0);
Texture2D ambientOcclusionTexture : register(t7, space0);

SamplerState materialSampler : register(s8, space0);

float3 getNormalFromTexture(float3 normal, float2 texcoord, float3 position)
{
    float3 tangentNormal = normalTexture.Sample(materialSampler, texcoord).xyz * 2.0 - 1.0;
    
    float3 Q1 = ddx(position);
    float3 Q2 = ddy(position);
    float2 st1 = ddx(texcoord);
    float2 st2 = ddy(texcoord);

    float3 N = normalize(normal);
    float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
    float3 B = -normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));

    return normalize(mul(TBN, tangentNormal));
}

float4 main(
    float3 viewPosition : POSITION0,
    float4 sVPosition : SV_POSITION,
    float3 position : POSITION1,
    float3 normal : NORMAL,
	float2 texcoord : TEXCOORD,
	uint   instanceId : SV_INSTANCEID) : SV_TARGET
{
    float3 toEye = normalize(transforms[instanceId].EyePosition.xyz - position);
	
	Material material;
	
	if (index.materialType == MATERIAL_BUFFER)
		material = materials[instanceId];
	else
	{
		material.DiffuseAlbedo = diffuseAlbedoTexture.Sample(materialSampler, texcoord);
        material.Metallic = metallicTexture.Sample(materialSampler, texcoord).r;
        material.Roughness = roughnessTexture.Sample(materialSampler, texcoord).r;
        material.AmbientOcclusion = ambientOcclusionTexture.Sample(materialSampler, texcoord).r;
	}


	float4 ambient = float4(
		index.ambientLightRed, 
		index.ambientLightGreen, 
		index.ambientLightBlue, 
		index.ambientLightAlpha) * material.DiffuseAlbedo * material.AmbientOcclusion;

    float4 color = ComputeLighting(lights.instance, material,
        position, getNormalFromTexture(normal, texcoord, position), toEye) + ambient;

    color = color / (color + 1.0f);
    color = pow(color, 1.0 / 2.2);

	return float4(color.xyz, material.DiffuseAlbedo.a);
}