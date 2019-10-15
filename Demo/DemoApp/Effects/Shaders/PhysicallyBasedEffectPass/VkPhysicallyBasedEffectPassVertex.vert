#version 450

#extension GL_ARB_separate_shader_objects : enable

struct Transform3D
{
    mat4 NormalTransform;
    mat4 Projection;
    mat4 Transform;
    mat4 View;
};

layout (set = 0, binding = 2) buffer Transform
{
    Transform3D instance[];
} transforms;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec3 outViewPosition;
layout (location = 1) out vec3 outPosition;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out uint outInstanceId;

void main()
{
    Transform3D transform = transforms.instance[gl_InstanceIndex];

    outPosition = (vec4(position, 1.0f) * transpose(transform.Transform)).xyz;
    outViewPosition = (vec4(outPosition, 1.0f) * transpose(transform.View)).xyz;
    outNormal = (vec4(normal, 1.0f) * transpose(transform.NormalTransform)).xyz;
    outInstanceId = gl_InstanceIndex;

    gl_Position = vec4(outViewPosition, 1.0f) * transpose(transform.Projection);
    gl_Position.y = -gl_Position.y;
}