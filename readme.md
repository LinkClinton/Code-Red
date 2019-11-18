# CodeRed

`CodeRed` is a Graphics Interface supports DirectX12 and Vulkan.

- Render anything instead of using Graphics API directly. 
- Do not need to take care of the different between DirectX12 and Vulkan.
- And it is easier to render or change the Graphics API.

## Documents

See documents folder or [click me](https://github.com/LinkClinton/Code-Red/blob/master/Documents/Readme.md).

## Installation

- Clone or download the repository.

```git
git clone https://github.com/LinkClinton/Code-Red.git
```
- Download and install the requisite SDK.
- Open the solution of `CodeRed` with [Visual Studio 2019](https://visualstudio.microsoft.com/).
- Build the source to a library(or reference the source).

## Requirement

- [Windows 10 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- C++ 17

The Windows SDK you can install with Visual Studio.

## Extensions

- [ImGui](https://github.com/LinkClinton/Code-Red/tree/master/Extensions/ImGui) : An ImGui backend implement using CodeRed.

## Tools

- [ShaderCompiler](https://github.com/LinkClinton/Code-Red/tree/master/Tools/ShaderCompiler) : A tool to compile shader to binary file or cpp array.

## Demos

There are some demos in [Code-Red-Demo](https://github.com/LinkClinton/Code-Red-Demo) repository.