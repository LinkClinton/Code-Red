#pragma pack_matrix(row_major)

struct View
{
    matrix view;
};

struct Output
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

ConstantBuffer<View> view : register(b0);

Output main(
    float2 position : POSITION,
    float2 texcoord : TEXCOORD)
{
    Output res;

    res.position = mul(float4(position, 0.0f, 1.0f), view.view);
    res.texcoord = texcoord;

    return res;
}