# Change Log

## 2019.12.25

- 2019.12.25 : Try Support Mip Levels. 
- Update Document.

## 2019.12.26

- 2019.12.26 : Add `GpuTextureRef` interface. 
- Update Document.

## 2019.12.28

- Add `GpuTextureBuffer` interface. 
- Remove `GpuGraphicsCommandList::copyMemoryToXXX`.
- Update Document.

## 2019.01.04

- Fix Vulkan Input Assembly State bug.
- Fix Vulkan Texture's flags.
- Fix Vulkan descriptor heaps(free same sets).
- Add Ignore message of Vulkan Debug Report.
- Fix some words.

## 2020.02.17

- Fix Vulkan Sampler Info.

## 2020.02.18

- Using Vulkan 1.1 for flipping the viewport.
- Add Compiler extension convert HLSL/GLSL to DXIL/SPIRV.
    - Support HLSL/GLSL to SPIRV.
- Update ImGui extension vulkan pixel shader (We use viewport to negative y-axis instead of shader).
- Update readme.

## 2020.02.19

- Support HLSL to DXIL.

## 2020.02.22

- Fix bug with depth-only rendering.
- Fix texture array. We use the length of texture reference to determine whether the texture is array. 

## 2020.02.23

- Change default frontface to `CounterClockwise`.
- Add PixelFormat to TextureRef. You can change the format when you bind a texture to rtv/dsv/srv.

## 2020.02.24

- Fix bug when we create the TextureRef.

## 2020.03.03

- Support multi-render target.
- Change Interface of `GpuFrameBuffer` and `GpuRenderPass`.

## 2020.03.04

- Fix blend state of multi-render target. The number of blend properties should be the number of frame buffers we render to.

## 2020.03.23

- Fix Vulkan version and debug report.
- Remove `ResourceLayout::Undefined`, the default layout of texture is `ResourceLayout::GeneralRead`.

## 2020.03.25

- Add document of Vulkan Validation Layer in Code-Red.
- Support MSAA Texture.

## 2020.03.26

- Add `GpuGraphicsCommandList::ResolveTexture` to resolve MSAA texture.
- Add `MultiSample` to `Attachment`.