# CodeRed-Tools-ShaderCompiler

ShaderCompiler is a program to compile the `hlsl` or `glsl` to binary file or cpp array. We can use it to compile the shader to a cpp array to avoid additional shader file.

## Build

Before you build the project, you need install the third parties we use. You can use `vcpkg` to install all libraries we use. You also need to make sure the `d3dcompiler.h` is available.

## ThirdParties

- [shaderc](https://github.com/google/shaderc) : A collection of tools, libraries, and tests for Vulkan shader compilation.

## Usage

- `-i file_name` : indicate the input file name.
- `-cpp array_name` : compile the shader code to cpp array.
- `-e entry_point` : indicate the entry point of hlsl(default : "main").
- `-o file_name` : indicate the output file name.
- `-v vertex/pixel` : indicate the shader type(default : vertex).
- `-a hlsl/glsl` : indicate the shader version(default : hlsl).

We use shader mode `5_1` to compile the hlsl shader without debug flag.