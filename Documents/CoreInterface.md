# Core Interface

- All Interfaces are `Noncopyable`. The copy constructer is disabled.
- We need to indicate which API version we want to create for all interfaces.
- We also can use device or factory to avoid the explicit API verison.

## Index

- [GpuLogicalDevice](#GpuLogicalDevice)
- [GpuSystemInfo](#GpuSystemInfo)
- [GpuDisplayAdapter](#GpuDisplayAdapter)
- [GpuCommandAllocator](#GpuCommandAllocator)
- [GpuGraphicsCommandList](#GpuGraphicsCommandList)
- [GpuCommandQueue](#GpuCommandQueue)
- [GpuSwapChain](#GpuSwapChain)
- [GpuFrameBuffer](#GpuFrameBuffer)
- [GpuRenderPass](#GpuRenderPass)
- [GpuTextureRef](#GpuTextureRef)
- [GpuResourceLayout](#GpuResourceLayout)
- [GpuDescriptorHeap](#GpuDescriptorHeap)
- [GpuGraphicsPipeline](#GpuGraphicsPipeline)

## GpuLogicalDevice

`GpuLogicalDevice` is the core interface in `CodeRed`. We need use it to create other interface except `GpuSystemInfo` and `GpuDisplayAdapter`. 

And it can decide which Graphics API we use. In fact, if we want use DirectX12, we only need to create a `DirectX12LogicalDevice` and use it to create other interfaces.

**Notice** : we recommend to use device to create other interfaces instead of using `new` or `std::make_shared`. **It can avoid the explicit API version requirement.**

### Constructer

```C++
    explicit GpuLogicalDevice(
        const std::shared_ptr<GpuDisplayAdapter>& adapter);
```

- `adapter` : adapter is a `GpuDisplayAdapter` object. It is the graphics card we want to use.

We can use this method to change the Graphics API we want to use. **And we must indicate which API version we want to use.**

```C++
#ifdef __DIRECTX12__MODE__
    auto device = std::make_shared<DirectX12LogicalDevice>(adapter);
#else
    auto device = std::make_shared<VulkanLogicalDevice>(adapter);
#endif
```

### Member Functions

All member functions of device is used to create other interfaces and return `shared_ptr<Interface>`(the shared pointer of interface).

 You can see more in `constructer` of other interfaces. 

## GpuSystemInfo

`GpuSystemInfo` is a simple and small interface to get some information of GPU before we create device. We can create this interface directly.

### Constructer

```C++
    explicit GpuSystemInfo();
```
**We must indicate which API version we want to use**.

```C++
#ifdef __DIRECTX12__MODE__
    auto systemInfo = std::make_shared<DirectX12SystemInfo>();
#else
    auto systemInfo = std::make_shared<VulkanSystemInfo>();
#endif
```

### Member Functions

- `selectDisplayAdapter()` : get all adapters we can use in this device.

## GpuDisplayAdapter

One `GpuDisplayAdapter` means one graphics card in the device. We can use `GpuSystemInfo` to get all graphics card we can use and choose one to create `GpuLogicalDevice`.

### Constructer

- **No Public Constructer**

We need to use `GpuSystemInfo` to get all adapters in this device.

```C++
    auto adapters = sysmteInfo.selectDisplayAdapter();
```

### Member Functions

- `name()` : get the graphics card name.
- `deviceId()` : get the graphics card device id.
- `vendorId()` : get the graphics card vendor id.

## GpuCommandAllocator

`GpuCommandAllocator` is a allocator for command list. All commands record by command list will store in command allocaotr.

### Constructer

```C++
    explicit GpuCommandAllocator(
        const std::shared_ptr<GpuLogicalDevice>& device);
```

- `device` : the device.

We recommend to use device to create command allocator.

```C++
    auto allocator = device->createCommandAllocator();
```

### Member Functions

- `reset` : clear the all comamnds in allocator. You need to ensure the command list are not recording commands.

## GpuGraphicsCommandList

A command list is used to recording the commands like draw, copy, set and so on. `GpuGraphicsCommandList` is used to recording the commands about graphics.

For example, We can use this list to command a draw command and use a command queue to submit the draw command to GPU. Then, GPU will solve this draw command and render something.

### Constructer

```C++
explicit GpuGraphicsCommandList(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::shared_ptr<GpuCommandAllocator>& allocator);
```

- `device` : the device.
- `allocator`: the allocator that store commands.

We recommend to use device to create command list.

```C++
    auto commandList = device->createGraphicsCommandList(allocator);
```

### Member Functions

Most member functions are same usage as DirectX12 and Vulkan. And some functions will introduce in other documents. **So there are only simple descriptions.** 

- `beginRecording()` : begin recording commands.
- `endRecording()` : end recording commands.
- `beginRenderPass()` : begin a render pass and set the frame buffer we want render to.
- `endRenderPass()` : end a render pass.
- `setGraphicsPipeline()` : set the graphics pipeline.
- `setResourceLayout()` : set the resource layout.
- `setVertexBuffer()` : set the vertex buffer.
- `setVertexBuffers()` : set the vertex buffers.
- `setIndexBuffer()` : set the index buffer.
- `setDescriptorHeap()` : set the descriptor heap.
- `setConstant32Bits()` : set the values of 32Bits.
- `setViewPort()` : set the view port.
- `setScissorRect()` : set the scissor rect.
- `layoutTransition()` : tranlate the layout of resource.
- `resolveTexture()` : resolve the MSAA texture.
- `copyBuffer()` : copy buffer from source to destination.
- `copyTexture()` : copy texture from source to destination.
- `copyTextureToBuffer()` : copy texture to buffer. 
- `copyBufferToTexture()` : copy buffer to texture.
- `draw()` : draw current vertex buffer.
- `draw()` : draw current vertex buffer with index buffer.

## GpuCommandQueue

A command queue is a queue that submit the commands to GPU. And GPU will solve the commands in order(FIFO like queue). The commands we submit are store in queue.

**We can have more than one command queue to submit commands, But the number limited by physical device**.

### Constructer

```C++
explicit GpuCommandQueue(
    const std::shared_ptr<GpuLogicalDevice>& device);
```

- `device` : the device.

We recommend to use device to create command queue.

```C++
    auto queue = device->createCommandQueue();
```

### Member Functions

- `execute()` : submit the command lists to GPU and execute them.
- `waitIdle()` : wait for the GPU to finishes the commands.

## GpuSwapChain

`GpuSwapChain` is used to present framebuffer to window. We create a swap chain connect textures to window. Then we can render something to window by rendering to texture.

### Constructer

```C++
explicit GpuSwapChain(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::shared_ptr<GpuCommandQueue>& queue,
    const WindowInfo& info,
    const PixelFormat& format,
    const size_t buffer_count = 2);
```

- `device` : the device.
- `queue` : the queue that will submit the present to GPU. 
- `info` : thw window info, include width, height and handle(HWND for windows).
- `format` : the format of back buffer.
- `buffer_count` : the numfer of back buffer. Two means we use double-buffer.

**Notice: If you want to render to back buffer(to window), you need to use the queue that create swap chain to submit draw commands.**

We recommend to use device to create swap chain.

```C++
    auto swapChain = device->createSwapChain(queue, info, format, 2);
```

### Member Functions

SwapChain is a typical and common object in Computer Graphics. 

**So there are only simple descriptions.**

- `buffer()` : get the back buffer by index.
- `bufferCount()` : get the number of back buffers.
- `info()` : get the window info.
- `width()` : get the width of back buffers(all back buffers have same width).
- `height()` : get the width of back buffers(all back buffers have same height).
- `format()` : get the format of back buffers(all back buffers have same format).
- `resize()` : resize the size of back buffers.
- `present()`: see more about `SwapChain` and `Double-Buffer`.
- `currentBufferIndex()` : get the current back buffer index. see more about `SwapChain` and `Double-Buffer`.

## GpuFrameBuffer

Frame buffer is the target we render. If we want to render something to texture, we need create a frame buffer for the texture and render to frame buffer. If we want to render to window, we need create a swap chain and get back buffers then create frame buffers for back buffers. 

**We can not render to texture or back buffers(They are also textures) directly.** We must create a frame bufer for them and render to frame buffer. 

**If we want to render with depth test or stencil, we need provide a depth-stencil buffer to frame buffer**.

See more in [RenderPass](./RenderPass.md).

### Constructer

```C++
explicit GpuFrameBuffer(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets,
    const std::shared_ptr<GpuTextureRef>& depth_stencil = nullptr);
```

- `device` : the device.
- `render_targets` : the texture we want to redner to.
- `depth_stencil` : the depth-stencil buffer we want to use.

We recommend to use device to create frame buffer.

```C++
    auto frameBuffer = device->createFrameBuffer({ texture }, nullptr);
```

### Member Functions

- `size()` : the number of render targets.
- `renderTarget()` : get the texture we want to render to.
- `depthStencil()` : get the depth-stencil buffer we use.
- `fullViewPort()` : default view port(full texture).
- `fullScissorRect()` : default scissor rect(full texture).

## GpuRenderPass

Render pass is a group of draw commands. We use `GpuGraphicsCommandList::beginRenderPass()` to begin a render pass and set the frame buffer. All draw commands will render to the frame buffer we set. 

When we end draw commands, we need to end render pass.

See more in [RenderPass](./RenderPass.md).

### Constructer

```C++
explicit GpuRenderPass(
    const std::shared_ptr<GpuLogicalDevice> &device, 
    const std::vector<Attachment>& colors,
    const std::optional<Attachment>& depth = std::nullopt);
```

- `device`: the device.
- `colors` : the property of texture we render to.
- `depth` : the property of depth-stencil we use.

We recommend to use device to create render pass.

```C++
    auto renderPass = device->createRenderPass(colors, std::nullopt);
```

**The property of colors and dpeth must be suit to the frame buffer we set in a render pass.**

### Member Functions

- `setClear()` : set the clear value.
- `colorClear` : get the clear value of color.
- `depthClear` : get the clear value of depth.
- `color()` : get color property.
- `depth()` : get depth-stencil property.
- `size()` : the number of color attachments.

## GpuTextureRef

Texture Ref is a class that reference some textures. A texture resource can have many sub-textures(texture array and mip levels). Sometimes, we do not need all sub-textures. So we can use `GpuTextureRef` to reference some sub-textures that we want to use.

**Notice : we usally use GpuTextureRef instead of GpuTexture to create frame buffer or bind to shader. If you use a GpuTexture to do this, it just create a `GpuTextureRef` reference all sub-textures.**

### Constructer

```C++
explicit GpuTextureRef(
    const std::shared_ptr<GpuTexture>& texture,
    const TextureRefInfo& info);
```

- `texture` : which texture we want to reference.
- `info` : how we reference the texture.

We recommend to use reference to create texture reference.

```C++
    const auto ref = texture.reference();
```

### TextureRefInfo

```C++
templpate<typename T>
struct ValueRange {
    T Start;
    T End;
}

struct TextureRefInfo {
    ValueRange<size_t> MipLevel;
    ValueRange<size_t> Array;

    TextureRefUsage Usage;
    PixelFormat Format;
```

- `MipLevel` : the mip levels we want to use.
- `Array` : the arrays we want to use.
- `Usage` : how do we use this. If you want to use it as CubeMap, the usage should be `TextureRefUsage::CubeMap`.
- `Format` : if you want to change the format when you bind a texture to rtv/dsv/srv, you can set the format to you want. Default is `Unknown` means use the format of texture.

**Notice : the size of array will influence the type of texture. If the size of array is not 1 for common texture(not cube map), the texture should be texture array. If the size of array greater 6 and the texture is cube map, it will be texture cube map array.**

## Member Functions

- `width` : get the width of first mip slice.
- `height` : get the height of first mip slice.
- `info` : get the `TextureRefInfo` property.
- `format` : get the format of texture.
- `mipLevel` : get the mip levels.
- `array` : get the arrays.
- `usage` : get the usage.
- `source` : get the texture we reference from.

## GpuResourceLayout

Resource layout is used to describe the layout of resource we want to bind to shader. 

See more in [Resource Binding](./ResourceBinding.md).

### Constructer

```C++
explicit GpuResourceLayout(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::vector<ResourceLayoutElement>& elements,
    const std::vector<SamplerLayoutElement>& samplers,
    const std::optional<Constant32Bits> &constant32Bits);
```

- `device` : the device.
- `elements` : the layout elements.
- `samplers` : the layout elements of sampler.
- `constant32Bits` : the property of constant32Bits.

We recommend to use device to create resource layout.

```C++
    //with empty resource layout.
    auto resourceLayout = device->createResourceLayout({}, {}, std::nullopt);
```

### Member Functions

- `element()` : get the layout element by index.
- `sampler()` : get the layout element of sampler by index.
- `elements()` : get the layout elements.
- `samplers()` : get the layout elements of sampler.
- `constant32Bits()` : get the property of constant32Bits.

## GpuDescriptorHeap

A descriptor heap is a set of descriptors. We bind the resource to descriptor heap and set the descriptor heap to graphics pipeline(shaders). In this way, we bind the resources to graphics pipeline(shaders) using descriptor heap.

A descriptor is create with resource layout. The resource layout indicate the layout of resource. We bind the resource to descriptor heap according to resource layout.

See more in [Resource Binding](./ResourceBinding.md).

### Constructer

```C++
explicit GpuDescriptorHeap(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::shared_ptr<GpuResourceLayout>& resource_layout);
```

- `device` : the device.
- `resource_layout` : the resource layout.

We recommend to use device to create descriptor heap.

```C++
    auto descriptorHeap = device->createDescriptorHeap(resourceLayout);
```

### Member Functions

- `bindResource()` : bind a resource to descriptor heap.
- `bindTexture()` : bind a texture to descriptor heap.
- `bindBuffer()` : bind a buffer to descriptor heap.
- `count()` : the number of elements in resource layout.
- `layout()` : the resource layout.

## GpuGraphicsPipeline

A graphics pipeline is a set of pipeline state. We bind some pipeline states like blend state, rasterization state, shader state to pipeline. 

A graphics pipeline indicate the state when we draw. Which shader we will use, if we enabled blend, enabled depth test and so on.

**Before we command a draw call, we need set a valid graphics pipeline to indicate the state of draw call. And the resource layout we set should be the one that create the graphics pipeline.**

### Constructer

```C++
explicit GpuGraphicsPipeline(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::shared_ptr<GpuRenderPass>& render_pass,
    const std::shared_ptr<GpuResourceLayout>& resource_layout,
    const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
    const std::shared_ptr<GpuShaderState>& vertex_shader_state,
    const std::shared_ptr<GpuShaderState>& pixel_shader_state,
    const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
    const std::shared_ptr<GpuBlendState>& blend_state,
    const std::shared_ptr<GpuRasterizationState>& rasterization_state);
```

**The parameters muse be valid. And we will introduce the parameters in PipelineState.**

We recommend to use device to create graphics pipeline.

```C++
    auto graphicsPipeline = device->createGraphicsPipeline(...);
```

### Member Functions

All member functions are used to get the state of graphics pipeline.