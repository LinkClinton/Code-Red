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
./vcpkg.exe install glm:x86-windows
```

- [shaderc](https://github.com/google/shaderc) : A tool to compile glsl to spv(demo only).

```git
./vcpkg.exe install shaderc:x64-windows
./vcpkg.exe install shaderc:x86-windows
```

## Demo

- [TriangleDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demo/TriangleDemo) : Triangle rendering and basic framework.
- [ParticlesDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demo/ParticlesDemo) : Texture sample, render to texture, blend.
- [FlowersDemo](https://github.com/LinkClinton/Code-Red/tree/master/Demo/FlowersDemo) : MSAA with shader.
- [DemoApp](https://github.com/LinkClinton/Code-Red/tree/master/Demo/DemoApp) : some help classes and functions.