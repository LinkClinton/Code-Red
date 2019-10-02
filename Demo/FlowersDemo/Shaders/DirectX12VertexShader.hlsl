#pragma pack_matrix(row_major)

struct TrianglePoints
{
    float2 positions[3];
};

struct TriangleColors
{
    float4 colors[3];
};

struct Output
{
    float4 color      : COLOR;
    float2 position   : POSITION;
    float4 svPosition : SV_POSITION;
    float2 texcoord   : TEXCOORD;
    uint instanceId   : SV_INSTANCEID;
};

struct View
{
    matrix view;
};

StructuredBuffer<TrianglePoints> trianglePoints : register(t0);
StructuredBuffer<TriangleColors> triangleColors : register(t1);
ConstantBuffer<View> view : register(b2);

Output main(float2 position : POSITION, uint vertexId : SV_VERTEXID, uint instanceId : SV_INSTANCEID)
{
    Output result;

    result.color = triangleColors[instanceId].colors[vertexId];
    result.position = trianglePoints[instanceId].positions[vertexId];
    result.svPosition = mul(float4(result.position, 0.0f, 1.0f), view.view);
    result.texcoord = position;
    result.instanceId = instanceId;

    return result;
}