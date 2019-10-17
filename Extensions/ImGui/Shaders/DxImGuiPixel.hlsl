SamplerState sampler0 : register(s1, space0);
Texture2D texture0 : register(t0, space0);

float4 main(
    float4 position : SV_POSITION,
    float4 color : COLOR,
    float2 uv : TEXCOORD) : SV_TARGET
{
    return color * texture0.Sample(sampler0, uv);
}