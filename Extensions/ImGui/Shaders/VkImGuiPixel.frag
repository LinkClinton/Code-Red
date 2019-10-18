#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (set = 0, binding = 0) uniform texture2D texture0;
layout (set = 0, binding = 1) uniform sampler sampler0;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inUv;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = inColor * texture(sampler2D(texture0, sampler0), inUv);
}