#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

#include "DirectX12Resource/DirectX12TextureBuffer.hpp"
#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12Resource/DirectX12Buffer.hpp"
#include "DirectX12GraphicsCommandList.hpp"
#include "DirectX12GraphicsPipeline.hpp"
#include "DirectX12CommandAllocator.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12DescriptorHeap.hpp"
#include "DirectX12LogicalDevice.hpp"
#include "DirectX12FrameBuffer.hpp"
#include "DirectX12RenderPass.hpp"
#include "DirectX12TextureRef.hpp"

#include "../Shared/DebugReport.hpp"

#undef min

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12GraphicsCommandList::DirectX12GraphicsCommandList(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator) :
	GpuGraphicsCommandList(device, allocator)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	const auto dxAllocator = static_cast<DirectX12CommandAllocator*>(mAllocator.get())->allocator();

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			dxAllocator.Get(), nullptr, IID_PPV_ARGS(&mGraphicsCommandList)),
		FailedException(DebugType::Create, { "ID3D12GraphicsCommandList" })
	);

	mGraphicsCommandList->Close();
}

void CodeRed::DirectX12GraphicsCommandList::beginRecording()
{
	mGraphicsCommandList->Reset(
		static_cast<DirectX12CommandAllocator*>(mAllocator.get())->allocator().Get(),
		nullptr);
	
	mGraphicsCommandList->ClearState(nullptr);

	mResourceLayout.reset();
}

void CodeRed::DirectX12GraphicsCommandList::endRecording()
{
	mGraphicsCommandList->Close();
}

void CodeRed::DirectX12GraphicsCommandList::beginRenderPass(
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuFrameBuffer>& frame_buffer)
{
	CODE_RED_DEBUG_THROW_IF(
		mFrameBuffer != nullptr ||
		mRenderPass != nullptr,
		Exception("please end old render pass before you begin a new render pass.")
	);

	mFrameBuffer = std::static_pointer_cast<DirectX12FrameBuffer>(frame_buffer);
	mRenderPass = std::static_pointer_cast<DirectX12RenderPass>(render_pass);
	
	CODE_RED_DEBUG_THROW_IF(
		!mRenderPass->compatible(mFrameBuffer),
		Exception("the render pass can not be compatible with frame buffer.")
	);
	
	const auto rtvAddress = mFrameBuffer->rtvHeap()->GetCPUDescriptorHandleForHeapStart();
	const auto dsvAddress = mFrameBuffer->dsvHeap()->GetCPUDescriptorHandleForHeapStart();

	const auto hasRTV = mFrameBuffer->size() != 0;
	const auto hasDSV = mFrameBuffer->depthStencil() != nullptr;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandle;
	
	CODE_RED_DEBUG_WARNING_IF(
		!hasRTV && !hasDSV,
		DebugReport::make(
			DebugType::Set,
			{ "FrameBuffer", "Graphics Pipeline" },
			{ "there are no rtv and dsv." }
		)
	);
	
	// begin layout transition
	for (size_t index = 0; index < mFrameBuffer->size(); index++) 
		tryLayoutTransition(mFrameBuffer->renderTarget(index), mRenderPass->color(index), false);

	tryLayoutTransition(mFrameBuffer->depthStencil(), mRenderPass->depth(), false);
	// end layout transition

	// begin clear the rtv and dsv if need
	for (size_t index = 0; index < mFrameBuffer->size(); index++) {
		const float color[] = {
			mRenderPass->colorClear()[index].Red,
			mRenderPass->colorClear()[index].Green,
			mRenderPass->colorClear()[index].Blue,
			mRenderPass->colorClear()[index].Alpha,
		};

		rtvHandle.push_back({ rtvAddress.ptr + index * mFrameBuffer->rtvSize() });
		
		CODE_RED_TRY_EXECUTE(
			mRenderPass->color(index)->Load == AttachmentLoad::Clear,
			mGraphicsCommandList->ClearRenderTargetView(rtvHandle[index],
				color, 0, nullptr)
		);
	}

	const auto depthAttachment = mRenderPass->depth();
	const auto clearValue = mRenderPass->depthClear();
	
	CODE_RED_TRY_EXECUTE(
		hasDSV && (
			depthAttachment->Load == AttachmentLoad::Clear ||
			depthAttachment->StencilLoad == AttachmentLoad::Clear),
		mGraphicsCommandList->ClearDepthStencilView(dsvAddress,
		(depthAttachment->Load == AttachmentLoad::Clear ? D3D12_CLEAR_FLAG_DEPTH : D3D12_CLEAR_FLAGS(0)) |
			(depthAttachment->StencilLoad == AttachmentLoad::Clear ? D3D12_CLEAR_FLAG_STENCIL : D3D12_CLEAR_FLAGS(0)),
			clearValue->Depth,
			clearValue->Stencil, 0, nullptr)
	);
	// end clear the rtv and dsv

	mGraphicsCommandList->OMSetRenderTargets(
		static_cast<UINT>(rtvHandle.size()),
		rtvHandle.data(),
		false,
		hasDSV ? &dsvAddress : nullptr
	);
}

void CodeRed::DirectX12GraphicsCommandList::endRenderPass()
{
	CODE_RED_DEBUG_THROW_IF(
		mFrameBuffer == nullptr ||
		mRenderPass == nullptr,
		Exception("please begin a render pass before end a render pass.")
	);

	for (size_t index = 0; index < mFrameBuffer->size(); index++)
		tryLayoutTransition(mFrameBuffer->renderTarget(index), mRenderPass->color(index), true);

	tryLayoutTransition(mFrameBuffer->depthStencil(), mRenderPass->depth(), true);

	mFrameBuffer.reset();
	mRenderPass.reset();
}

void CodeRed::DirectX12GraphicsCommandList::setGraphicsPipeline(
	const std::shared_ptr<GpuGraphicsPipeline>& pipeline)
{
	mGraphicsCommandList->SetPipelineState(
		static_cast<DirectX12GraphicsPipeline*>(pipeline.get())->pipeline().Get()
	);

	mGraphicsCommandList->IASetPrimitiveTopology(
		enumConvert(pipeline->inputAssembly()->primitiveTopology()));
}

void CodeRed::DirectX12GraphicsCommandList::setResourceLayout(const std::shared_ptr<GpuResourceLayout>& layout)
{
	const auto dxLayout = std::static_pointer_cast<DirectX12ResourceLayout>(layout);
	
	mGraphicsCommandList->SetGraphicsRootSignature(
		dxLayout->rootSignature().Get()
	);

	mResourceLayout = dxLayout;
}

void CodeRed::DirectX12GraphicsCommandList::setVertexBuffer(const std::shared_ptr<GpuBuffer>& buffer)
{
	D3D12_VERTEX_BUFFER_VIEW view = {
		static_cast<DirectX12Buffer*>(buffer.get())->buffer()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->size()),
		static_cast<UINT>(buffer->stride())
	};
	
	mGraphicsCommandList->IASetVertexBuffers(0, 1, &view);
}

void CodeRed::DirectX12GraphicsCommandList::setVertexBuffers(
	const std::vector<std::shared_ptr<GpuBuffer>>& buffers,
	const size_t startSlot)
{
	auto views = std::vector<D3D12_VERTEX_BUFFER_VIEW>(buffers.size());

	for (size_t index = 0; index < views.size(); index++) {
		const auto& buffer = std::static_pointer_cast<DirectX12Buffer>(buffers[index]);

		views[index].BufferLocation = buffer->buffer()->GetGPUVirtualAddress();
		views[index].StrideInBytes = static_cast<UINT>(buffer->stride());
		views[index].SizeInBytes = static_cast<UINT>(buffer->size());
	}

	mGraphicsCommandList->IASetVertexBuffers(
		static_cast<UINT>(startSlot),
		static_cast<UINT>(views.size()),
		views.data());
}

void CodeRed::DirectX12GraphicsCommandList::setIndexBuffer(
	const std::shared_ptr<GpuBuffer>& buffer,
	const IndexType type)
{
	D3D12_INDEX_BUFFER_VIEW view = {
		static_cast<DirectX12Buffer*>(buffer.get())->buffer()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->size()),
		enumConvert(type)
	};
	
	mGraphicsCommandList->IASetIndexBuffer(&view);
}

void CodeRed::DirectX12GraphicsCommandList::setDescriptorHeap(
	const std::shared_ptr<GpuDescriptorHeap>& heap)
{
	CODE_RED_DEBUG_THROW_IF(
		heap->layout() != mResourceLayout,
		FailedException(DebugType::Set,
			{ "GpuDescriptorHeap", "Graphics Pipeline" }, 
			{ "current resource layout is not the one that create the heap." });
	);

	const auto dxHeap = std::static_pointer_cast<DirectX12DescriptorHeap>(heap)->heap();
	
	mGraphicsCommandList->SetDescriptorHeaps(1, dxHeap.GetAddressOf());

	CODE_RED_TRY_EXECUTE(
		heap->count() != 0,
		mGraphicsCommandList->SetGraphicsRootDescriptorTable(
			static_cast<UINT>(mResourceLayout->elementsIndex()),
			dxHeap->GetGPUDescriptorHandleForHeapStart())
	);
}

void CodeRed::DirectX12GraphicsCommandList::setConstant32Bits(
	const std::vector<Value32Bit>& values)
{
	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout == nullptr,
		FailedException(DebugType::Set,
			{ "Constant32Bits", "Graphics Pipeline" },
			{ "please set the resource layout, before set constant32Bits." })
	);

	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout->constant32Bits().has_value() == false,
		FailedException(DebugType::Set,
			{ "Constant32Bits", "Graphics Pipeline" },
			{ "please enable the Constant32Bits in GpuResourceLayout." })
	)
	
	CODE_RED_DEBUG_WARNING_IF(
		mResourceLayout->constant32Bits()->Count < values.size(),
		DebugReport::make(DebugType::Set,
			{ "Constant32Bits", "Graphics Pipeline"},
			{
				"the size of values is greater than the count of Constant32Bits."
				"We will only set values[0] to values[count - 1]."
			})
	);

	mGraphicsCommandList->SetGraphicsRoot32BitConstants(
		static_cast<UINT>(mResourceLayout->constant32BitsIndex()),
		static_cast<UINT>(std::min(values.size(), mResourceLayout->constant32Bits()->Count)),
		values.data(),
		0
	);
}

void CodeRed::DirectX12GraphicsCommandList::setViewPort(const ViewPort& view_port)
{
	D3D12_VIEWPORT viewPort = {
		view_port.X,
		view_port.Y,
		view_port.Width,
		view_port.Height,
		view_port.MinDepth,
		view_port.MaxDepth
	};
	
	mGraphicsCommandList->RSSetViewports(1, &viewPort);
}

void CodeRed::DirectX12GraphicsCommandList::setScissorRect(const ScissorRect& rect)
{
	D3D12_RECT scissorRect = {
		static_cast<LONG>(rect.Left),
		static_cast<LONG>(rect.Top),
		static_cast<LONG>(rect.Right),
		static_cast<LONG>(rect.Bottom)
	};
	
	mGraphicsCommandList->RSSetScissorRects(1, &scissorRect);
}

void CodeRed::DirectX12GraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuTextureBuffer>& buffer,
	const ResourceLayout old_layout, 
	const ResourceLayout new_layout)
{
	CODE_RED_DEBUG_THROW_IF(
		buffer->layout() != old_layout,
		InvalidException<ResourceLayout>({ "old_layout" })
	);

	auto barrier = resourceBarrier(
		static_cast<DirectX12TextureBuffer*>(buffer.get())->texture().Get(),
		enumConvert(old_layout),
		enumConvert(new_layout)
	);

	mGraphicsCommandList->ResourceBarrier(1, &barrier);

	buffer->setLayout(new_layout);
}


void CodeRed::DirectX12GraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuTexture>& texture,
	const ResourceLayout old_layout, 
	const ResourceLayout new_layout)
{
	CODE_RED_DEBUG_THROW_IF(
		texture->layout() != old_layout,
		InvalidException<ResourceLayout>({ "old_layout" })
	);

	auto barrier = resourceBarrier(
		static_cast<DirectX12Texture*>(texture.get())->texture().Get(),
		enumConvert(old_layout),
		enumConvert(new_layout)
	);
	
	mGraphicsCommandList->ResourceBarrier(1, &barrier);

	texture->setLayout(new_layout);
}

void CodeRed::DirectX12GraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuBuffer>& buffer,
	const ResourceLayout old_layout, 
	const ResourceLayout new_layout)
{
	CODE_RED_DEBUG_THROW_IF(
		buffer->layout() != old_layout,
		InvalidException<ResourceLayout>({ "old_layout" })
	);
	
	auto barrier = resourceBarrier(
		static_cast<DirectX12Buffer*>(buffer.get())->buffer().Get(),
		enumConvert(old_layout),
		enumConvert(new_layout)
	);

	mGraphicsCommandList->ResourceBarrier(1, &barrier);

	buffer->setLayout(new_layout);
}

void CodeRed::DirectX12GraphicsCommandList::copyBuffer(
	const std::shared_ptr<GpuBuffer>& source,
	const std::shared_ptr<GpuBuffer>& destination, 
	const size_t size, 
	const size_t source_offset,
	const size_t destination_offset)
{
	const auto dxSource = static_cast<DirectX12Buffer*>(source.get())->buffer();
	const auto dxDestination = static_cast<DirectX12Buffer*>(destination.get())->buffer();

	mGraphicsCommandList->CopyBufferRegion(
		dxDestination.Get(),
		static_cast<UINT64>(destination_offset),
		dxSource.Get(),
		static_cast<UINT64>(source_offset),
		static_cast<UINT64>(size)
	);
}

void CodeRed::DirectX12GraphicsCommandList::copyTexture(
	const std::shared_ptr<GpuTexture>& source,
	const std::shared_ptr<GpuTexture>& destination,
	const Extent3D<size_t>& region, 
	const size_t x,
	const size_t y,
	const size_t z)
{
	copyTexture(
		TextureCopyInfo(source, region.Left, region.Top, region.Front),
		TextureCopyInfo(destination, x, y, z),
		region.Right - region.Left,
		region.Bottom - region.Top,
		region.Back - region.Front
	);
}

void CodeRed::DirectX12GraphicsCommandList::copyTexture(
	const TextureCopyInfo& source,
	const TextureCopyInfo& destination, 
	const size_t width, const size_t height, const size_t depth)
{
	const auto dxSource = std::static_pointer_cast<DirectX12Texture>(source.Texture)->texture();
	const auto dxDestination = std::static_pointer_cast<DirectX12Texture>(destination.Texture)->texture();

	D3D12_TEXTURE_COPY_LOCATION src;
	D3D12_TEXTURE_COPY_LOCATION dst;

	src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	src.pResource = dxSource.Get();
	src.SubresourceIndex = static_cast<UINT>(source.ResourceIndex);

	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.pResource = dxDestination.Get();
	dst.SubresourceIndex = static_cast<UINT>(destination.ResourceIndex);

	D3D12_BOX srcRegion = {
		static_cast<UINT>(source.LocationX),
		static_cast<UINT>(source.LocationY),
		static_cast<UINT>(source.LocationZ),
		static_cast<UINT>(source.LocationX + width),
		static_cast<UINT>(source.LocationY + height),
		static_cast<UINT>(source.LocationZ + depth)
	};

	mGraphicsCommandList->CopyTextureRegion(&dst,
		static_cast<UINT>(destination.LocationX),
		static_cast<UINT>(destination.LocationY),
		static_cast<UINT>(destination.LocationZ),
		&src,
		&srcRegion);
}

void CodeRed::DirectX12GraphicsCommandList::copyTextureToBuffer(
	const TextureCopyInfo& source,
	const TextureBufferCopyInfo& destination, 
	const size_t width, const size_t height, const size_t depth)
{
	const auto dxSource = std::static_pointer_cast<DirectX12Texture>(source.Texture)->texture();
	const auto dxDestination = std::static_pointer_cast<DirectX12TextureBuffer>(destination.Buffer)->texture();

	D3D12_TEXTURE_COPY_LOCATION src;
	D3D12_TEXTURE_COPY_LOCATION dst;

	src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	src.pResource = dxSource.Get();
	src.SubresourceIndex = static_cast<UINT>(source.ResourceIndex);

	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.pResource = dxDestination.Get();
	dst.SubresourceIndex = 0;

	D3D12_BOX srcRegion = {
		static_cast<UINT>(source.LocationX),
		static_cast<UINT>(source.LocationY),
		static_cast<UINT>(source.LocationZ),
		static_cast<UINT>(source.LocationX + width),
		static_cast<UINT>(source.LocationY + height),
		static_cast<UINT>(source.LocationZ + depth)
	};

	mGraphicsCommandList->CopyTextureRegion(&dst,
		static_cast<UINT>(0),
		static_cast<UINT>(0),
		static_cast<UINT>(0),
		&src,
		&srcRegion);
}

void CodeRed::DirectX12GraphicsCommandList::copyBufferToTexture(
	const TextureBufferCopyInfo& source,
	const TextureCopyInfo& destination, 
	const size_t width, const size_t height, const size_t depth)
{
	const auto dxSource = std::static_pointer_cast<DirectX12TextureBuffer>(source.Buffer)->texture();
	const auto dxDestination = std::static_pointer_cast<DirectX12Texture>(destination.Texture)->texture();

	D3D12_TEXTURE_COPY_LOCATION src;
	D3D12_TEXTURE_COPY_LOCATION dst;

	src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	src.pResource = dxSource.Get();
	src.SubresourceIndex = 0;

	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.pResource = dxDestination.Get();
	dst.SubresourceIndex = static_cast<UINT>(destination.ResourceIndex);

	D3D12_BOX srcRegion = {
		static_cast<UINT>(0),
		static_cast<UINT>(0),
		static_cast<UINT>(0),
		static_cast<UINT>(width),
		static_cast<UINT>(height),
		static_cast<UINT>(depth)
	};

	mGraphicsCommandList->CopyTextureRegion(&dst,
		static_cast<UINT>(destination.LocationX),
		static_cast<UINT>(destination.LocationY),
		static_cast<UINT>(destination.LocationZ),
		&src,
		&srcRegion);
}

void CodeRed::DirectX12GraphicsCommandList::draw(
	const size_t vertex_count, 
	const size_t instance_count,
	const size_t start_vertex_location, 
	const size_t start_instance_location)
{
	mGraphicsCommandList->DrawInstanced(
		static_cast<UINT>(vertex_count),
		static_cast<UINT>(instance_count),
		static_cast<UINT>(start_vertex_location),
		static_cast<UINT>(start_instance_location)
	);
}

void CodeRed::DirectX12GraphicsCommandList::drawIndexed(
	const size_t index_count, 
	const size_t instance_count,
	const size_t start_index_location, 
	const size_t base_vertex_location, 
	const size_t start_instance_location)
{
	mGraphicsCommandList->DrawIndexedInstanced(
		static_cast<UINT>(index_count),
		static_cast<UINT>(instance_count),
		static_cast<UINT>(start_index_location),
		static_cast<UINT>(base_vertex_location),
		static_cast<UINT>(start_instance_location)
	);
}

D3D12_RESOURCE_BARRIER CodeRed::DirectX12GraphicsCommandList::resourceBarrier(
	ID3D12Resource* pResource,
	const D3D12_RESOURCE_STATES before, 
	const D3D12_RESOURCE_STATES after)
{

	D3D12_RESOURCE_BARRIER barrier = {};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	return barrier;
}

void CodeRed::DirectX12GraphicsCommandList::tryLayoutTransition(
	const std::shared_ptr<GpuTextureRef>& texture,
	const std::optional<Attachment>& attachment, 
	const bool final)
{
	CODE_RED_TRY_EXECUTE(
		texture != nullptr && attachment.has_value(),
		layoutTransition(texture->source(),texture->source()->layout(),
			final ? attachment->FinalLayout : attachment->InitialLayout)
	);
}

#endif
