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

layout (set = 0, binding = 2) uniform ViewType
{
    mat4 view;
} view;

layout (location = 0) in vec2 pos;

layout (location = 0) out vec4 color;
layout (location = 1) out vec2 position;
layout (location = 2) out vec2 texcoord;
layout (location = 3) out uint instanceId; 

void main()
{
    color = triangleColors.data[gl_InstanceIndex].colors[gl_VertexIndex];
    position = trianglePoints.data[gl_InstanceIndex].positions[gl_VertexIndex];
    texcoord = pos;
    instanceId = gl_InstanceIndex;

    gl_Position = vec4(position, 0.0f, 1.0f) * transpose(view.view);
    gl_Position.y = -gl_Position.y;
}