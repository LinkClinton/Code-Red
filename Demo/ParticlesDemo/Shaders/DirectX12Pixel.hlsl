#pragma pack_matrix(row_major)

Texture2D particleTexture : register(t2);
SamplerState particleSampler : register(s3);

float4 main(
    float4 position : SV_POSITION,
    float2 texcoord : TEXCOORD,
    uint   identity : SV_INSTANCEID) : SV_TARGET
{
    float alpha = particleTexture.Sample(particleSampler, texcoord);

    if (alpha <= 0.0f) discard;

    return float4(1.0f, 0.0f, 0.0f, alpha);
}