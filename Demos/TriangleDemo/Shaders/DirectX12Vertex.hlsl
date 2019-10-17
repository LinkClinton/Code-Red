#pragma pack_matrix(row_major)

struct View
{
    matrix view;
};

ConstantBuffer<View> view : register(b0);

float4 main(float3 position : POSITION) : SV_POSITION
{
    return mul(float4(position, 1.0f), view.view);
}