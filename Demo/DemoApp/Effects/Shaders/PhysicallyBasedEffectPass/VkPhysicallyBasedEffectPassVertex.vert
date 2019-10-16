#version 450

#extension GL_ARB_separate_shader_objects : enable

struct Transform3D
{
    mat4 NormalTransform;
    mat4 Projection;
    mat4 Transform;
    mat4 View;
    vec4 EyePosition;
};

layout (set = 0, binding = 2) buffer Transform
{
    Transform3D instance[];
} transforms;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 tangent;

layout (location = 0) out vec3 outViewPosition;
layout (location = 1) out vec3 outPosition;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec2 outTexcoord;
layout (location = 4) out vec3 outTangent;
layout (location = 5) out uint outInstanceId;

void main()
{
    Transform3D transform = transforms.instance[gl_InstanceIndex];

    outPosition = (transform.Transform * vec4(position, 1.0)).xyz;
    outViewPosition = (transform.View * vec4(outPosition, 1.0)).xyz;
    outNormal = mat3(transform.NormalTransform) * normal;
    outTangent = mat3(transform.NormalTransform) * tangent;
    outTexcoord = texcoord;
    outInstanceId = gl_InstanceIndex;

    gl_Position = transform.Projection * vec4(outViewPosition, 1.0f);
    gl_Position.y = -gl_Position.y;
}