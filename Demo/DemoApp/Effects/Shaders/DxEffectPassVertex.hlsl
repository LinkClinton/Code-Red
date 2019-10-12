#pragma pack_matrix(row_major)

#define MAX_LIGHTS_PER_TYPE 16
#define MAX_ALL_LIGHTS MAX_LIGHTS_PER_TYPE * 3
#define MAX_MATERIALS MAX_LIGHTS_PER_TYPE
#define MAX_TRANSFORMS MAX_LIGHTS_PER_TYPE

struct Transform3D
{
    matrix NormalTransform;
    matrix Projection;
    matrix Transform;
    matrix View;
};

struct Transforms 
{
    Transform3D instance[MAX_TRANSFORMS];
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

struct Output
{
    float3 ViewPosition : POSITION0;
    float4 SVPosition : SV_POSITION;
    float3 Position : POSITION1;
    float3 Normal : NORMAL;
};

ConstantBuffer<Transforms> transforms : register(b2, space0);
ConstantBuffer<Index> index : register(b3, space0); 

Output main(
    float3 position : POSITION,
    float3 normal : NORMAL)
{
    Output result;

    uint transformIndex = index.transformIndex;

    result.Position = mul(float4(position, 1.0f), transforms.instance[transformIndex].Transform).xyz;
    result.ViewPosition = mul(float4(result.Position, 1.0f), transforms.instance[transformIndex].View).xyz;
    result.SVPosition = mul(float4(result.ViewPosition, 1.0f), transforms.instance[transformIndex].Projection);
    result.Normal = mul(float4(normal, 1.0f), transforms.instance[transformIndex].NormalTransform).xyz;
    
    return result;
}