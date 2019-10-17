#pragma pack_matrix(row_major)

float4 main(
    float4 position : SV_POSITION,
    float2 texcoord : TEXCOORD) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f) * (1.01f - length(texcoord));
}