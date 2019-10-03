#pragma pack_matrix(row_major)

struct Transform
{
    matrix world;
};

struct Output
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    uint   identity : SV_INSTANCEID;
};

struct View
{
    matrix view;
};

ConstantBuffer<View> view : register(b0);
StructuredBuffer<Transform> transforms : register(t1);

Output main(
    float2 position : POSITION,
    float2 texcoord : TEXCOORD,
    uint   identity : SV_INSTANCEID)
{
    Output res;

    res.position = mul(float4(position, 0.0f, 1.0f), transforms[identity].world);
    res.position = mul(res.position, view.view);
    res.texcoord = texcoord;
    res.identity = identity;

    return res;
}