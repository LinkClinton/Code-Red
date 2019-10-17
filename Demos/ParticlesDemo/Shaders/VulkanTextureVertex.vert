#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (set = 0, binding = 0) uniform View
{
    mat4 view;
} view;

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec2 outTexcoord;

void main()
{
    gl_Position = position * transpose(view.view);
    gl_Position.y = -gl_Position.y;

    outTexcoord = texcoord; 
}