# CodeRed

`CodeRed` is an Graphics Interface supports DirectX12 and Vulkan.

- Render anything instead of using Graphics API directly. 
- Do not need to take care of the different between DirectX12 and Vulkan.
- And it is easier to render or change the Graphics API.

## Document

See document folder or [click me](https://github.com/LinkClinton/Code-Red/tree/master/Document/readme.md).

## Installation

- Clone or download the repository.

```git
git clone https://github.com/LinkClinton/Code-Red.git
```
- Download and install the requisite SDK.
- Download and install third party library using [Vcpkg](https://github.com/Microsoft/vcpkg). 
- Open the solution of `CodeRed` with [Visual Studio 2019](https://visualstudio.microsoft.com/).
- Build the source to a library(or reference the source).

## Requirement

- [Windows 10 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- C++ 17
- [Python 3](https://www.python.org/)(demo only)

The Windows SDK you can install with Visual Studio.

## Third Party

- [glm](https://github.com/g-truc/glm): A math library(demo only).

```git
./vcpkg.exe install glm:x64-windows
```

- [shaderc](https://github.com/google/shaderc) : A tool to compile glsl to spv(demo only).

```git
./vcpkg.exe install shaderc:x64-windows
```
- [stb](https://github.com/nothings/stb) : we use stb_image to load texture(demo only).

```git
./vcpkg.exe install stb:x64-windows
```

## Demos

- [TriangleDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demos/TriangleDemo) : Triangle rendering and basic framework.
- [ParticlesDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demos/ParticlesDemo) : Texture sample, render to texture, blend.
- [FlowersDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demos/FlowersDemo) : MSAA with shader.
- [EffectPassDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demos/EffectPassDemo) : Lighting, PBR with effect pass.
- [DemoApp](https://github.com/LinkClinton/Code-Red/tree/master/Demos/DemoApp) : some help classes and functions.