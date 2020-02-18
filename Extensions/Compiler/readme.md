# Compiler

This extension can convert HLSL/GLSL to DXIL/SPIRV.

## Feature

- HLSL to SPIRV ✔
- GLSL to SPIRV ✔
- HLSL to DXIL
- GLSL to DXIL

## Usage

Just full `CompileOption` and compile shader code.

## Notice 

If you want to use root constant(push_constant), you need add `[[vk::push_constant]]` to your HLSL code before `ConstantBuffer`.

For example, you have a root constant at `resgister(b0, space0)`.

```HLSL
[[vk::push_constant]] ConstantBuffer<T> NAME : register(b0, space0);
```

## ThirdParties

- `shaderc` : https://github.com/google/shaderc

We recommend you to use `vcpkg` to install these third-parties.