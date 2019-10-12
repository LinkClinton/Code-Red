#pragma pack_matrix(row_major)

#define MAX_LIGHTS_PER_TYPE 16
#define MAX_ALL_LIGHTS MAX_LIGHTS_PER_TYPE * 3
#define MAX_MATERIALS MAX_LIGHTS_PER_TYPE
#define MAX_TRANSFORMS MAX_LIGHTS_PER_TYPE

struct Material
{
    float4 DiffuseAlbedo;
    float3 FresnelR0;
    float Roughness;
};

struct Light {
    float3 Strength;
    float FalloffStart; 
    float3 Direction; 
    float FalloffEnd; 
    float3 Position; 
    float SpotPower; 
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVector){
    float cosIncidentAngle = saturate(dot(normal, lightVector));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVector, float3 normal, float3 toEye, Material material)
{
    const float m = (1.0f - material.Roughness) * 256.0f;
    float3 halfVec = normalize(toEye + lightVector);

    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(material.FresnelR0, halfVec, lightVector);
    float3 specAlbedo = fresnelFactor * roughnessFactor;

    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (material.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light light, Material material, float3 normal, float3 toEye)
{
    if (light.Strength.x == 0 && light.Strength.y == 0 && light.Strength.z == 0) return float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = -light.Direction;

    float ndotl = max(dot(lightVector, normal), 0.0f);
    float3 lightStrength = light.Strength * ndotl;

    return BlinnPhong(lightStrength, lightVector, normal, toEye, material);
}

float3 ComputePointLight(Light light, Material material, float3 position, float3 normal, float3 toEye)
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

    return BlinnPhong(lightStrength, lightVector, normal, toEye, material);
}

float3 ComputeSpotLight(Light light, Material material, float3 position, float3 normal, float3 toEye)
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

    return BlinnPhong(lightStrength, lightVector, normal, toEye, material);
}

float4 ComputeLighting(Light lights[MAX_ALL_LIGHTS], Material material, float3 position, float3 normal, float3 toEye)
{
    normal = normalize(normal);

    float3 result = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < MAX_LIGHTS_PER_TYPE; i++)
    {
        result = result + ComputeDirectionalLight(lights[0 * MAX_LIGHTS_PER_TYPE + i], material, normal, toEye);
        result = result + ComputePointLight(lights[1 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye);
        result = result + ComputeSpotLight(lights[2 * MAX_LIGHTS_PER_TYPE + i], material, position, normal, toEye);
    }

    return float4(result.xyz, material.DiffuseAlbedo.a);
}

struct Lights
{
    Light instance[MAX_ALL_LIGHTS];
};

struct Materials
{
    Material instance[MAX_MATERIALS];
};

struct Index
{
    uint materialIndex;
    uint transformIndex;
	float ambientLightRed;
	float ambientLightGreen;
	float ambientLightBlue;
	float ambientLightAlpha;
};

ConstantBuffer<Lights> lights : register(b0, space0);
ConstantBuffer<Materials> materials : register(b1, space0);
ConstantBuffer<Index> index : register(b3, space0);

float4 main(
    float3 viewPosition : POSITION0,
    float4 sVPosition : SV_POSITION,
    float3 position : POSITION1,
    float3 normal : NORMAL) : SV_TARGET
{
    uint materialIndex = index.materialIndex;
    
    float3 toEye = float3(0.0f, 0.0f, 0.0f) - viewPosition;
    
	float4 ambient = float4(
		index.ambientLightRed, 
		index.ambientLightGreen, 
		index.ambientLightBlue, 
		index.ambientLightAlpha) * materials.instance[materialIndex].DiffuseAlbedo;

    float4 color = ComputeLighting(lights.instance, materials.instance[materialIndex],
        position, normal, toEye) + ambient;

	return float4(color.xyz, materials.instance[materialIndex].DiffuseAlbedo.a);
}