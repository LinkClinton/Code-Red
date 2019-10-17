#version 450

#extension GL_ARB_separate_shader_objects : enable

struct TrianglePoints
{
    vec2 positions[3];
};

struct TriangleColors
{
    vec4 colors[3];
};

layout (set = 0, binding = 0) buffer TrianglePointsType
{
    TrianglePoints data[];
} trianglePoints;

layout (set = 0, binding = 1) buffer TriangleColorsType
{
    TriangleColors data[];
} triangleColors;

float msaa_cross(vec2 u, vec2 v) 
{
	return u.x * v.y - u.y * v.x;
}

float msaa_area_function(vec2 u, vec2 v, vec2 p)
{
	return msaa_cross(v - u, p - u);
}

float msaa_sample(vec2 position, 
	vec2 position0, vec2 position1, vec2 position2,
	vec4 color0, vec4 color1, vec4 color2) 
{
	float inv_triangle_area = abs(1.0f / msaa_area_function(position0, position1, position2));

	float sub_area0 = abs(msaa_area_function(position1, position2, position)) * inv_triangle_area;
	float sub_area1 = abs(msaa_area_function(position2, position0, position)) * inv_triangle_area;
	float sub_area2 = abs(msaa_area_function(position0, position1, position)) * inv_triangle_area;

	vec2 uv = vec2(0, 0) * sub_area0 + vec2(0.5f, 0) * sub_area1 + vec2(1, 1) * sub_area2;
	
	if (uv.x * uv.x - uv.y > 0) return 0;

	return color0.a * sub_area0 + color1.a * sub_area1 + color2.a * sub_area2;
}

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 position;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in flat uint instanceId;

layout (location = 0) out vec4 target;

void main()
{
    float edge_function = texcoord.x * texcoord.x - texcoord.y;

    //only enable mass at the edge
	if (abs(edge_function) < 0.01) 
	{
		vec2   offset[4];
		float  sampled[4];

		offset[0] = vec2(0.25f, 0.25f);
		offset[1] = vec2(0.25f, -0.25f); 
		offset[2] = vec2(-0.25f, 0.25f); 
		offset[3] = vec2(-0.25f, -0.25f);

		for (int i = 0; i < 4; i++) 
		{
			sampled[i] = msaa_sample(position + offset[i],
				trianglePoints.data[instanceId].positions[0],
				trianglePoints.data[instanceId].positions[1],
				trianglePoints.data[instanceId].positions[2],
				triangleColors.data[instanceId].colors[0],
				triangleColors.data[instanceId].colors[1],
				triangleColors.data[instanceId].colors[2]);
		}

		target = vec4(color.xyz, (sampled[0] + sampled[1] + sampled[2] + sampled[3]) * 0.25f);

        return;
	}

    if (edge_function > 0) discard;

	target = color;
}