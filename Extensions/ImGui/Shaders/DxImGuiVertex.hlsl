struct Index
{
    float4x4 Projection;
};

struct Output
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

ConstantBuffer<Index> transform : register(b2, space0); 

Output main(
    float2 position : POSITION,
    float4 color : COLOR,
    float2 uv : TEXCOORD)
{
    Output output;

    output.Position = mul(transform.Projection, float4(position, 0.f, 1.f));
    output.Color = color;
    output.UV = uv;

    return output;
}