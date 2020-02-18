#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (push_constant) uniform Index
{
	mat4 Projection;
} index;

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inUv;
layout (location = 2) in vec4 inColor;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUv;

void main()
{
    outColor = inColor;
    outUv = inUv;

    gl_Position = index.Projection * vec4(inPosition, 0.0, 1.0);
}