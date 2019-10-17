# Resource Layout

Resource has a state that indicate what the resource will do or what the resource we want it do. Before we do something, we need to translate the resource's layout to the layout that support the things we want to do.

For example, We create a frame buffer and a render pass, then we set them. Before we record some draw command, we need translate the render target in frame buffer to `ResourceLayout::RenderTarget` and translate the depth stencil in frame buffer to `ResourceLayout::DepthStencil`(In fact, this operator will finish in begin and end of render pass, so we do not do this).

If you want to copy a buffer, you need translate the source of buffer to `ResourceLayout::CopySource` and translate the destination to `ResourceLayout::CopyDestination`.

```C++
    enum class ResourceLayout : UInt32
    {
        Undefined,
        GeneralRead,
        RenderTarget,
        DepthStencil,
        CopyDestination,
        CopySource,
        Present
    };
```

- `Undefined` : undefined.
- `GeneralRead` : used for GPU read.
- `RenderTarget` : used for render target.
- `DepthStencil` : used for depth stencil.
- `CopyDestination` : used for copy destination.
- `CopySource` : used for copy source.
- `Present` : used for present.

## LayoutTransition

```C++
    void GpuGraphicsCommandList::layoutTransition(
        const std::shared_ptr<GpuBuffer>& buffer,
        const ResourceLayout old_layout,
        const ResourceLayout new_layout);

    void GpuGraphicsCommandList::layoutTransition(
        const std::shared_ptr<GpuTexture>& texture,
        const ResourceLayout old_layout,
        const ResourceLayout new_layout);
```

- `buffer` : the resource we want to translate.
- `texture` : the resource we want to translate.
- `old_layout` : the old layout of resource.
- `new_layout` : the new layout of resource.