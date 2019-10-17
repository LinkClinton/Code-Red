#pragma pack_matrix(row_major)

struct Color {
	float4 color;
};

ConstantBuffer<Color> color : register(b1);

float4 main(float4 position : SV_POSITION) : SV_TARGET
{
	return color.color;
}