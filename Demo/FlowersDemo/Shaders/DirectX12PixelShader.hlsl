#pragma pack_matrix(row_major)

struct TrianglePoints
{
    float2 positions[3];
};

struct TriangleColors
{
    float4 colors[3];
};

StructuredBuffer<TrianglePoints> trianglePoints : register(t0);
StructuredBuffer<TriangleColors> triangleColors : register(t1);

float msaa_cross(float2 u, float2 v) 
{
	return u.x * v.y - u.y * v.x;
}

float msaa_area_function(float2 u, float2 v, float2 p)
{
	return msaa_cross(v - u, p - u);
}

float msaa_sample(float2 position, 
	float2 position0, float2 position1, float2 position2,
	float4 color0, float4 color1, float4 color2) 
{
	float inv_triangle_area = abs(1.0f / msaa_area_function(position0, position1, position2));

	float sub_area0 = abs(msaa_area_function(position1, position2, position)) * inv_triangle_area;
	float sub_area1 = abs(msaa_area_function(position2, position0, position)) * inv_triangle_area;
	float sub_area2 = abs(msaa_area_function(position0, position1, position)) * inv_triangle_area;

	float2 uv = float2(0, 0) * sub_area0 + float2(0.5f, 0) * sub_area1 + float2(1, 1) * sub_area2;
	
	if (uv.x * uv.x - uv.y > 0) return 0;

	return color0.a * sub_area0 + color1.a * sub_area1 + color2.a * sub_area2;
}

float4 main(
    float4 color      : COLOR,
    float2 position   : POSITION,
    float4 svPosition : SV_POSITION,
    float2 texcoord   : TEXCOORD,
    uint instanceId   : SV_INSTANCEID) : SV_TARGET
{
    float edge_function = texcoord.x * texcoord.x - texcoord.y;

	//only enable mass at the edge
	if (abs(edge_function) < 0.01) 
	{
		float2 offset[4];
		float  sampled[4];

		offset[0] = float2(0.25f, 0.25f);
		offset[1] = float2(0.25f, -0.25f); 
		offset[2] = float2(-0.25f, 0.25f); 
		offset[3] = float2(-0.25f, -0.25f);

		for (int i = 0; i < 4; i++) 
		{
			sampled[i] = msaa_sample(position + offset[i],
				trianglePoints[instanceId].positions[0],
				trianglePoints[instanceId].positions[1],
				trianglePoints[instanceId].positions[2],
				triangleColors[instanceId].colors[0],
				triangleColors[instanceId].colors[1],
				triangleColors[instanceId].colors[2]);
		}

		return float4(color.xyz, (sampled[0] + sampled[1] + sampled[2] + sampled[3]) * 0.25f);
	}

	if (edge_function > 0) discard;

	return color;
}