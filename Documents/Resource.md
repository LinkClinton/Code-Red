# Resource

- All resources inherit from `GpuResource`(except `GpuSampler`).
- All resources are `Noncopyable`. The copy constructer is disabled.
- We need to indicate which API version we want to create for all resources.
- We recommend to use `GpuLogicalDevice` to create all resouces to avoid the explicit API version.

## Index

- [ResourceInfo](#ResourceInfo)
- [GpuBuffer](#GpuBuffer)
- [GpuTexture](#GpuTexture)
- [GpuSampler](#GpuSampler)
- [GpuTextureBuffer](#GpuTextureBuffer)

## ResourceInfo

`ResourceInfo` is a struct shared with DirectX12 and Vulkan interface. We use this struct to create all resources(except `GpuSampler`).

### Constructer

```C++
    ResourceInfo(
        const ResourceProperty& property,
        const ResourceLayout layout,
        const ResourceUsage usage,
        const ResourceType type,
        const MemoryHeap heap);
```

- `property` : the property of resource. It is a std::variant class that hold two different properties(buffer and texture). We will introduce it soon.
- `layout` : layout is the current state of resource. See more in [ResourceLayout](./ResourceLayout.md).
- `usage` : the usage of resource, such as vertex buffer, render target and so on.
- `type` : resource type, such as buffer, texture and so on.
- `heap` : default or upload. Deafult means we can not mapped the memory to CPU(but we can copy data from resource to them). Upload means we can mapped memory to CPU and copy data from CPU to them.

**Notice : the heap of texture must be default.**

### Member Functions

There are some static member functions to help us create resource easier. For example, we can use `ResourceInfo::Texture2D(...)` to create a `ResourceInfo` used to create `GpuTexture`.

## GpuBuffer

There are two kind of buffers. 

- `ResourceType::Buffer` : simple buffer.
- `ResourceType::GroupBuffer` : array of buffers.

### Constructer

```C++
    explicit GpuBuffer(
        const std::shared_ptr<GpuLogicalDevice>& device,
        const ResourceInfo& info);
```

- `device` : the device.
- `info` : the resource info.

We recommend to use `GpuLogicalDevice` to create buffer.

```C++
    auto buffer = device->createBuffer(...);
```

### BufferProperty

```C++
    struct BufferProperty {
        size_t Stride;
        size_t Count;
        size_t Size;
    };
```

- `Stride` : the size of element in buffer.
- `Count` : the number of elements in buffer.
- `Size` : the size of elements in buffer(Stride * Count).

**Notice: the size of buffer must greater than 256bytes. If the size is less than 256bytes, we will create a buffer with 256bytes.**

The `Count` of buffer with usage `ConstantBuffer` must be one. And the `Count` of group buffer or buffer with usage `VertexBuffer` or `IndexBuffer` can greater than one .

### Member Functions

All member functions is used to get informations of buffer or mapped memory.

## GpuTexture

Texture has three dimension.

- `Texture1D` : only has width property.
- `Texture2D` : only has width, height properties.
- `Texture3D` : has width, height, depth properties.

For `Texture1D` and `Texture2D` the depth property is array size.

If the resource type of texture is `CubeMap`, it just create a texture 2d array.

### Constructer

```C++
explicit GpuTexture(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const ResourceInfo& info)
```

- `device` : the device.
- `info` : the resource info.

We recommend to use `GpuLogicalDevice` to create texture.

```C++
    auto texture = device->createTexture(...);
```

### TextureProperty

```C++
    struct TextureProperty {
        size_t Width;
        size_t Height;
        size_t Depth;
        size_t Size;
        
        size_t MipLevels;

        PixelFormat lFormat;
        Dimension Dimension;
    };
```

- `Width` : the width of texture(origin).
- `Height` : the height of texture(origin).
- `Depth` : the depth of texture or array size of texture.
- `Size` : the size of texture(origin).
- `MipLevels` : the max number of mip levels.
- `Format` : the format of pixel in texture.
- `Dimension` : the dimension of texture(1D, 2D, 3D).
- `ClearValue` : the clear value used in clear rtv/dsv operation.

The size of texture is the size of origin texture. It is not the size of this objects. Because this objects can be texture array or has mip levels. 

For `Texture1D` the height must be one.

**Notice : the layout of texture must be `ResourceLayout::Undefined` when we create a texture.**

If you are using rtv\dsv in DirectX12 mode, you can set the `TextureProperty::ClearValue` to optimize the clear operation(**the value you used in clear operation should be same as the value you set**).

### Resource Index

A texture can have many textures, for example, we can have a texture that has texture array and with mip levels. So we give this sub-textures a index. It is equal to `mipSlice + arraySlice * mipLevels`. 

If you have a texture array with max mip levels 5 and you want to get the 4-th texture with mip levels 3, so the index is `3 + 4 * 5 = 23`.(The way to encode `mipSlice` and `arraySlice` is same as the way in DirectX12)

### Member Functions

All member functions is used to get informations of texture or mapped memory.

- If you want to get texture property like width, height, depth and size with mip level, you can set the `mipSlice` with these functions(`width`, `height`, `depth`, `size`). **Notice, the depth of array is always the number of texture array.**

- If you want to update memory to texture, you should use `GpuGraphicsCommandList::copyMemoryToTexture`.

- The size of texture is not always same in different adapters, so we add `GpuTexture::physicalSize` and `GpuTexture::alignment` to indicate the real size of texture and the layout of alignment.

### Copy Texture

It is not legal to copy a texture array or all texture with mip levels. But you can copy element of texture(you can copy entire texture with multi-copy operation).

```C++
struct TextureCopyInfo {
    std::shared_ptr<GpuTexture> Texture;
    size_t ResourceIndex;
    size_t LocationX;
    size_t LocationY;
    size_t LocationZ;
}

void copyTexture(
    const TextureCopyInfo& source,
    const TextureCopyInfo& destination,
    const size_t width,
    const size_t height,
    const size_t depth = 1);
```

`TextureCopyInfo` is a help structure for copying. The resource index is the sub-texture in texture.

And the `Location` indicate the start position we copy from or to.

## GpuSampler

Sampler is used to sample the data from texture. **You can learn more from DirectX or Vulkan.**

### Constructer

```C++
    explicit GpuSampler(
        const std::shared_ptr<GpuLogicalDevice>& device,
        const SamplerInfo& info)
```

- `device` : the device;
- `info` : the sampler info.

We recommend to use `GpuLogicalDevice` to create sampler.

```C++
    auto sampler = device->createSampler(...);
```

### SamplerInfo

```C++
    struct SamplerInfo {
        FilterOptions Filter;
        AddressMode AddressModeU;
        AddressMode AddressModeV;
        AddressMode AddressModeW;
        
        BorderColor Border;
        size_t MaxAnisotropy;
    };
```

**You can learn more from DirectX, Vulkan and Texture Sample.**

### Member Functions

All member functions is used to get informations of sampler.

## GpuTextureBuffer

Texture Buffer just a buffer that we can read data from CPU and write data to CPU. We can not read data from CPU or write data to CPU with a `Texture`. But we can copy texture to Texture Buffer, then we can read the texture data. We also can copy the Texture Buffer to texture.

### Constructer

```C++
explicit GpuTextureBuffer(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const TextureBufferInfo& info);

explicit GpuTextureBuffer(
    const std::shared_ptr<GpuLogicalDevice>& device,
    const std::shared_ptr<GpuTexture>& texture,
    const size_t mipSlice);
```

- `device` : the device.
- `info` : the texture buffer info.

We also can create it from a texture with mip level. It will fill the `TextureBufferInfo` struct with its width, height, depth, format.

We recommend to use `GpuLogicalDevice` to create texture buffer.

```C++
    auto texture = device->createTextureBuffer(...);
```

### TextureBufferInfo 

```C++
struct TextureBufferInfo {
    size_t Width;
    size_t Height;
    size_t Depth;

    size_t Size;

    PixelFormat Format;
    Dimension Dimension;
    ResourceLayout Layout;
};
```

- `Width` : the width of texture.
- `Height` : the height of texture.
- `Depth` : the depth of texture.
- `Size` : the size of texture.
- `Format` : the format of texture.
- `Dimension` : the dimension of texture.
- `Layout` : the layout of texture.

Most values are same as the values in `TextureProperty`. But the `TextureBuffer` can not have sub-textures(No Array, No MipLevels).

### Member Functions

Most member functions is used to get informations of sampler.

- `read` : read data from buffer.
- `write` : write data to buffer.

### Read and Write Texture

We can not read or write texture directly. If you want to read texture, you need copy texture to `TextureBuffer` and read data from `TextureBuffer`. If you want to write texture, you need write data to `TextureBuffer` and copy `TextureBuffer` to texture. 

So we have two copy functions of `TextureBuffer`.

```C++
virtual void copyTextureToBuffer(
    const TextureCopyInfo& source,
    const TextureBufferCopyInfo& destination,
    const size_t width,
    const size_t height,
    const size_t depth = 1) = 0;

virtual void copyBufferToTexture(
    const TextureBufferCopyInfo& source,
    const TextureCopyInfo& destination,
    const size_t width,
    const size_t height,
    const size_t depth = 1) = 0;
```

**Notice : the Location of `TextureBuffer` is 0.**
