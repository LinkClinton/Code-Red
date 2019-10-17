#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (set = 0, binding = 2) uniform texture2D particleTexture;
layout (set = 0, binding = 3) uniform sampler particleSampler;

layout (location = 0) in vec2 texcoord;

layout (location = 0) out vec4 outColor;

void main()
{
    vec4 alpha = texture(sampler2D(particleTexture, particleSampler), texcoord);

    if (alpha.a <= 0.0f) discard;

    outColor = vec4(1.0f, 0.0f, 0.0f, alpha.a);
}