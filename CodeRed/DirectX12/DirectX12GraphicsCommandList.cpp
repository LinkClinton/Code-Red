#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

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
	)
	
	mFrameBuffer = std::static_pointer_cast<DirectX12FrameBuffer>(frame_buffer);
	mRenderPass = std::static_pointer_cast<DirectX12RenderPass>(render_pass);

	const auto clearValue = mRenderPass->getClear();
	const auto colorAttachment = mRenderPass->color();
	const auto depthAttachment = mRenderPass->depth();

	const Real color[] = {
		clearValue.first.Red,
		clearValue.first.Green,
		clearValue.first.Blue,
		clearValue.first.Alpha };
	
	//we can set a frame buffer without rtv or dsv
	//but we will send a warning if we do not have rtv and dsv
	const auto has_rtv = mFrameBuffer->renderTarget() != nullptr && colorAttachment.has_value();
	const auto has_dsv = mFrameBuffer->depthStencil() != nullptr && depthAttachment.has_value();
	const auto rtvAddress = mFrameBuffer->rtvHeap()->GetCPUDescriptorHandleForHeapStart();
	const auto dsvAddress = mFrameBuffer->dsvHeap()->GetCPUDescriptorHandleForHeapStart();
		
	CODE_RED_DEBUG_WARNING_IF(
		has_rtv == false && has_dsv == false,
		DebugReport::make(
			DebugType::Set,
			{ "FrameBuffer", "Graphics Pipeline" },
			{ "there are no rtv and dsv." }
		)
	);

	tryLayoutTransition(mFrameBuffer->renderTarget(), colorAttachment, false);
	tryLayoutTransition(mFrameBuffer->depthStencil(), depthAttachment, false);

	CODE_RED_TRY_EXECUTE(
		has_rtv && colorAttachment->Load == AttachmentLoad::Clear,
		mGraphicsCommandList->ClearRenderTargetView(rtvAddress, color, 0, nullptr)
	);

	CODE_RED_TRY_EXECUTE(
		has_dsv && (
			depthAttachment->Load == AttachmentLoad::Clear ||
			depthAttachment->StencilLoad == AttachmentLoad::Clear),
		mGraphicsCommandList->ClearDepthStencilView(dsvAddress,
			(depthAttachment->Load == AttachmentLoad::Clear ? D3D12_CLEAR_FLAG_DEPTH : D3D12_CLEAR_FLAGS(0)) |
			(depthAttachment->StencilLoad == AttachmentLoad::Clear ? D3D12_CLEAR_FLAG_STENCIL : D3D12_CLEAR_FLAGS(0)),
			clearValue.second.Depth,
			clearValue.second.Stencil, 0, nullptr)
	);
	
	mGraphicsCommandList->OMSetRenderTargets(
		has_rtv ? 1 : 0,
		has_rtv ? &rtvAddress : nullptr,
		false,
		has_dsv ? &dsvAddress : nullptr);
}

void CodeRed::DirectX12GraphicsCommandList::endRenderPass()
{
	CODE_RED_DEBUG_THROW_IF(
		mFrameBuffer == nullptr ||
		mRenderPass == nullptr,
		Exception("please begin a render pass before end a render pass.")
	)
	
	const auto colorAttachment = mRenderPass->color();
	const auto depthAttachment = mRenderPass->depth();

	tryLayoutTransition(mFrameBuffer->renderTarget(), colorAttachment, true);
	tryLayoutTransition(mFrameBuffer->depthStencil(), depthAttachment, true);

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
	const auto dxSource = static_cast<DirectX12Texture*>(source.get())->texture();
	const auto dxDestination = static_cast<DirectX12Texture*>(destination.get())->texture();

	D3D12_TEXTURE_COPY_LOCATION src;
	D3D12_TEXTURE_COPY_LOCATION dst;

	src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	src.pResource = dxSource.Get();
	src.SubresourceIndex = 0;

	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.pResource = dxDestination.Get();
	dst.SubresourceIndex = 0;

	D3D12_BOX srcRegion = {
		static_cast<UINT>(region.Left),
		static_cast<UINT>(region.Top),
		static_cast<UINT>(region.Front),
		static_cast<UINT>(region.Right),
		static_cast<UINT>(region.Bottom),
		static_cast<UINT>(region.Back)
	};
	
	mGraphicsCommandList->CopyTextureRegion(&dst,
		static_cast<UINT>(x),
		static_cast<UINT>(y),
		static_cast<UINT>(z),
		&src,
		&srcRegion);
}

void CodeRed::DirectX12GraphicsCommandList::copyMemoryToBuffer(
	const std::shared_ptr<GpuBuffer>& destination,
	const void* data)
{
	const auto dxAllocator = std::static_pointer_cast<DirectX12CommandAllocator>(mAllocator);

	const auto buffer = dxAllocator->allocateCopyCacheBuffer(destination->size());

	const auto memory = buffer->mapMemory();
	std::memcpy(memory, data, destination->size());
	buffer->unmapMemory();
	
	copyBuffer(buffer, destination, destination->size(), 0, 0);
}

void CodeRed::DirectX12GraphicsCommandList::copyMemoryToTexture(
	const std::shared_ptr<GpuTexture>& destination,
	const void* data)
{
	const auto dxAllocator = std::static_pointer_cast<DirectX12CommandAllocator>(mAllocator);

	const auto texture = dxAllocator->allocateCopyCacheTexture(
		std::get<TextureProperty>(destination->info().Property));
	const auto dxTexture = std::static_pointer_cast<DirectX12Texture>(texture);

	const auto rowPitch = PixelFormatSizeOf::get(texture->format()) * texture->width();
	const auto depthPitch = rowPitch * texture->height();
	
	dxTexture->texture()->WriteToSubresource(0, nullptr, data,
		static_cast<UINT>(rowPitch),
		static_cast<UINT>(depthPitch));

	copyTexture(texture, destination,
		Extent3D<size_t>(
			0, 0, 0,
			texture->width(),
			texture->height(),
			texture->depth()
			), 0, 0, 0);
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
	const std::shared_ptr<GpuTexture>& texture,
	const std::optional<Attachment>& attachment, 
	const bool final)
{
	CODE_RED_TRY_EXECUTE(
		texture != nullptr && attachment.has_value(),
		layoutTransition(texture,texture->layout(),
			final ? attachment->FinalLayout : attachment->InitialLayout)
	);
}

#endif
