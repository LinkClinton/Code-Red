# PipelineState

- All pipeline states inherit from `GpuPipelineState`.
- All pipeline states are `Noncopyable`. The copy constructer is disabled.
- We need to indicate which API version we want to create for all pipeline states.
- We recommend to use `GpuPipelineFactory` to create all pipeline states to avoid the explicit API version.
- All pipeline state is used to describe the state of graphics pipeline or compute pipeline.

## Index

- [GpuInputAssemblyState](#GpuInputAssemblyState)
- [GpuRasterizationState](#GpuRasterizationState)
- [GpuDepthStencilState](#GpuDepthStencilState)
- [GpuShaderState](#GpuShaderState)
- [GpuBlendState](#GpuBlendState)
- [GpuPipelineFactory](#GpuPipelineFactory)

## GpuInputAssemblyState

Input assembly state defines the layout of input vertex and the primitive topology type. Layout of input vertex means the components(Position, Color, normal and so on) of vertex when we command a draw call. Primitive topology type means how we use the vertices to build primitive such as line, point, triangle and so on.

### Constructer

```C++
explicit GpuInputAssemblyState(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology);
```

- `device` : the device.
- `elements` : elements to describe the layout of input vertex.
- `primitive_topology` : how to build primitive with vertices.

We recommend to use `GpuPipelineFactory` to create input assembly state.

```C++
    auto inputAssemblyState = pipelineFactory->createInputAssemblyState(elements, topology);
```

### InputLayoutElement

```C++
struct InputLayoutElement {
    PixelFormat Format;
    std::string Name;
    size_t Slot;
}
```

- `Format` : the element's format and size.
- `Name` : the element's name.
- `Slot` : indicate which slot the element from.

`InputLayoutElement` is shared with DirectX12 and Vulkan interface. It is used to define the layout of input vertex. The number of element in `elements`(**in constructer state**) is the number of components in vertex. **The n-th element describe the n-th component of vertex.**

`InputLayoutElement::Slot` indicate the data of element or component of vertex from which vertex buffer. We can set more than one vertex buffers in one draw call. In fact, when we set vertex buffers, we will set a start slot, the first vertex buffers will use start slot, the second will use start slot + 1...

For example, if we have two elements, the first is "Position" and its slot is 1 and the second is "Color" and its slot is 2. So we need two vertex buffers, the first should contains the data of "Position" and the second should contains the data of "Color". And the `startSlot` we set should be 1.

**For Slot, you can learn more from DirectX or Vulkan. Slot called Binding in Vulkan.**

Format describe the size in bytes and type of component. Name is a unique identity(Only used in DirectX12 mode, in HLSL).

### Member Functions

All member functions are used to get the informations of input assembly state(elements, primitive topology type and so on).

## GpuRasterizationState

Rasterization state is used to describe how we rasterize the objects we draw. Such as if we enable back cull, depth clamp and so on(**You can learn more about rasterization from DirectX or Vulkan**).

### Constructer

```C++
    explicit GpuRasterizationState(
        const std::shared_ptr<GpuLogicalDevice>& device,
        const FrontFace front_face,
        const CullMode cull_mode,
        const FillMode fill_mode,
        const bool depth_clamp);
```

- `front_face` : which face is front(CounterClockwise or Clockwise).
- `cull_mode` : which face should be culled(front, back or both).
- `fill_mode` : how we filled the primitive.
- `depth_clamp` : enable depth clamp.

We recommend to use `GpuPipelineFactory` to create rasterization state.

```C++
    auto rasterization = pipelineFactory->createRasterizationState(...);
```

### Member Functions

All member functions are used to get the informations of rasterization state.

## GpuDepthStencilState

DepthStencil state is used to describe the state of depth test and stencil. **You can learn more about depth-stencil state from DirectX or Vulkan.**

### Constructer

```C++
	explicit GpuDepthStencilState(
        const std::shared_ptr<GpuLogicalDevice> &device,
        const bool depth_enable,
        const bool depth_write_enable,
        const bool stencil_enable,
        const CompareOperator depth_operator,
        const StencilOperatorInfo& front,
        const StencilOperatorInfo& back); 
```

Most parameters are same as interface in DirectX12 or Vulkan. You can learn more from them.

We recommend to use `GpuPipelineFactory` to create depthStencil state.

```C++
    auto depthStencilState = pipelineFactory->createDepthStencilState(...);
```

### Member Functions

All member functions are used to get the informations of depth stencil state.

## GpuShaderState

Shader state is used to set shader code we want use in the pipeline. We set the shader code into shader state and set shader state into pipeline.

### Constructer

```C++
    explicit GpuShaderState(
        const std::shared_ptr<GpuLogicalDevice>& device,
        const ShaderType type,
        const std::vector<Byte>& code,
        const std::string& name);
```

- `device` : the device.
- `type` : the shader type(vertex, pixel or others).
- `code` : the compiled shader code.
- `name` : the entry point of shader.

We recommend to use `GpuPipelineFactory` to create shader state.

```C++
    auto vertexShaderState = pipelineFactory->createShaderState(ShaderType::Vertex, code, "main");
```

### Member Functions

All member functions are used to get the informations of shader state.

## GpuBlendState

Blend state is ued to describe how we blend the pixel we generate in pixel shader stage with the pixel in frame buffer. **You can learn more about blend state from DirectX or Vulkan.**

### Constructer

```C++
    explicit GpuBlendState(
        const std::shared_ptr<GpuLogicalDevice> &device,
        const std::vector<BlendProperty>& blend_properties); 
```

`blend_properties` are same as interface in DirectX12 or Vulkan. You can learn more from them.

We recommend to use `GpuPipelineFactory` to create blend state.

```C++
    auto blendState = pipelineFactory->createBlendState({ property });
```

### Member Functions

All member functions are used to get the informations of blend state.

## GpuPipelineFactory

Pipeline factory is used to create the pipeline state and we can use it to avoid the explicit API version. The version of states are decided by the version of `GpuPipelineFactory`. If the pipeline factory is `DirectX12PipelineFactory`, the states are also `DirectX12...State`. 

### Constructer

```C++
    explicit GpuPipelineFactory(
        const std::shared_ptr<GpuLogicalDevice> &device);
```

- `device` : the device.

We recommend to use `GpuLogicalDevice` to create the pipeline factory.

```C++
    auto pipelineFactory = device->createPipelineFactory();
```

### Member Functions

All member functions are used to create the pipeline state.