# Resource Binding

Resource bindding will introduce how we define the resource layout in C++ code and Shader code. And how we bind the resource to pipeline, to shader.

## GpuResourceLayout

It is a interface to describe the resource layout of binding. 

The second parameter of constructer is `elements` which describe the layout of `Buffer` and `Texture`.

The third parameter of constructer is `samplers` which describe the layout of `Sampler`.

**Notice : we only support static sampler object, so we need bind the sampler resource to resource layout when we create a GpuResourceLayout.**

### ResourceLayoutElement

```C++
struct ResourceLayoutElement {
    ShaderVisibility Visibility;
    ResourceType Type;
    size_t Binding;
    size_t Space;
}
```

- `Visibility` : which shader can see the resource. 
- `Type` : the type of resource we want to bind to.
- `Binding` : the binding.
- `Space` : the space.

The `Visibility` only support in Vulkan mode, it always be `ShaderVisibility::All` in DirectX12 mode.

The `Binding` in HLSL is `register` and in GLSL is `binding`.

The `Space` in HLSL is `sapce` and in GLSL is `set`.

**Notice : in a resource layout, can not have two elements in `elements` or `samplers` that have same binding and space.**

### ResourceType

```C++
    enum class ResourceType : UInt32
    {
        Buffer,
        Texture,
        GroupBuffer
    };
```

There is a table describe the relation of `ResourceType`, `HLSL`, `GLSL`.

| ResourceType | HLSL             | GLSL    |
| ------------ | ---------------- | ----    |
| Buffer       | ConstantBuffer   | uniform |
| Texture      | Texture          | texture |
| GroupBuffer  | StructuredBuffer | buffer  |

**Notice: in HLSL or GLSL, the Texture need add the dimension information. For example, a Texture2D in HLSL is Texture2D, in GLSL is texture2D.**

### Example

We have a buffer(binding = 2, space = 3), gourp buffer(binding = 1, space = 2), texture(binding = 10, space = 3), sampler(binding = 0, space = 0). 

In C++, the code is :

```C++
    auto resourceLayout = device->CreateResourceLayout(
        {
            CodeRed::ResourceLayoutElement(ResourceType::Buffer, 2, 3),
            CodeRed::ResourceLayoutElement(ResourceType::GroupBuffer, 1, 2),
            CodeRed::ResourceLayoutElement(ResourceType::Texture, 10, 3)
        },
        {   CodeRed::SamplerLayoutElement(sampler, 0, 0) }
    );
```

in HLSL, the code is :

```HLSL
ConstantBuffer<Buffer_Element_Type> Buffer_Name : register(b2, space3);
StructuredBuffer<Buffer_Element_Type> Buffer_Name : register(b1, space2);
Texture2D Texture_Name : register(t10, space3);
SamplerState Sampler_Name : register(s0, space0);
```

in GLSL, the code is :

```GLSL
layout (set = 3, binding = 2) uniform Type_Name { DataType... } Buffer_Name;
layout (set = 2, binding = 1) buffer Type_Name { Buffer_Element_Type Name[]; } Buffer_Name;
layout (set = 3, binding = 10) uniform texture2D Texture_Name;
layout (set = 0, binding = 0) uniform sampler Sampler_Name;
```

## GpuDescriptorHeap

Descriptor heap is a set of descriptor. We need to bind the resource to heap. And we set the heap to pipeline, to shader. The descriptor heap is create from resource layout. **When we set a descriptor heap, the resource layout we set must be the one that create the heap**.

### Bind Resource

```C++
    void GpuDescriptorHeap::bindResource(
        const std::shared_ptr<GpuResource>& resource,
        const size_t index);
```

- `resource` : the resource we want to bind.
- `index` : the index of elements that in resource layout's constructer.

Index that describe we want to bind the resource to which resource in shader. For example(see example in Resource Layout), if we bind a resource to with index 1, means we bind a resource as `GroupBuffer` with binding 1, space 2. If we bind a resource to with index 2, means we bind a resource as `Texture` with binding 10, space 3.

## Constant32Bits

We also can set some values of 32Bits to shader without descriptor heap. But we need set the `constant32Bits` at constructer of `GpuResourceLayout`.

```C++
struct Constant32Bits {
    ShaderVisibility Visibility;
    size_t Binding;
    size_t Space;
    size_t Count;
}
```
- `Visibility` : which shader can see the resource. 
- `Binding` : the binding.
- `Space` : the space.
- `Count` : the number of 32Bit-values.

We can use `GpuGraphicsCommandList::setConstant32Bits` to set some 32Bit-values to shader. 

```C++
    commandList->setConstant32Bits({ 0.0f, 1.0f, 0 });
```

**Notice : if you want to set constant 32Bit-values, you need a resource layout that `std::optional<Constant32Bits>` is not `std::nullopt` at construction stage.**

### Shader Code

In HLSL, the code is :

```HLSL
struct Structure_Name
{
    float value0;
    float value1;
    int value2;
};

ConstantBuffer<Structure_Name> Value_Name : register(binding, space);
```

In GLSL, the code is :

```GLSL
layout(push_constant) uniform Structure_Name
{
    float value0;
    float value1;
    int value2;
} Value_Name;
```