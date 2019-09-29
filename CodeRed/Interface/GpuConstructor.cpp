#include "../Shared/Exception/ZeroException.hpp"

#include "GpuResource/GpuTexture.hpp"
#include "GpuResource/GpuSampler.hpp"
#include "GpuResource/GpuBuffer.hpp"

#include "GpuGraphicsCommandList.hpp"
#include "GpuCommandAllocator.hpp"
#include "GpuGraphicsPipeline.hpp"
#include "GpuResourceLayout.hpp"
#include "GpuDisplayAdapter.hpp"
#include "GpuDescriptorHeap.hpp"
#include "GpuLogicalDevice.hpp"
#include "GpuCommandQueue.hpp"
#include "GpuFrameBuffer.hpp"
#include "GpuRenderPass.hpp"
#include "GpuSwapChain.hpp"
#include "GpuFence.hpp"

#include <algorithm>

#undef max

#define CODE_RED_DEBUG_DEVICE_VALID(device) \
	CODE_RED_DEBUG_THROW_IF( \
		device == nullptr, \
		ZeroException<GpuLogicalDevice>({ "device" })); \

#define CODE_RED_DEBUG_PTR_VALID(ptr, name) \
	CODE_RED_DEBUG_THROW_IF( \
		ptr == nullptr, \
		ZeroException<void>( { name }));

CodeRed::GpuSwapChain::GpuSwapChain(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandQueue>& queue,
	const WindowInfo& info,
	const PixelFormat& format, 
	const size_t buffer_count) :
	mDevice(device),
	mQueue(queue),
	mBuffers(buffer_count),
	mInfo(info),
	mPixelFormat(format)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_PTR_VALID(queue, "queue");
	
	CODE_RED_DEBUG_THROW_IF(
		mInfo.handle == nullptr || mInfo.width == 0 || mInfo.height == 0,
		ZeroException<WindowInfo>({ "info" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mBuffers.size() == 0,
		ZeroException<size_t>({ "buffer_count" })
	);
}

CodeRed::GpuResourceLayout::GpuResourceLayout(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers) :
	mDevice(device),
	mElements(elements),
	mSamplers(samplers)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuLogicalDevice::GpuLogicalDevice(
	const std::shared_ptr<GpuDisplayAdapter>& adapter) :
	mDisplayAdapter(adapter)
{
	CODE_RED_DEBUG_THROW_IF(
		mDisplayAdapter == nullptr,
		ZeroException<GpuDisplayAdapter>({ "adapter" })
	);
}

CodeRed::GpuGraphicsCommandList::GpuGraphicsCommandList(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator) :
	mDevice(device),
	mAllocator(allocator)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		mAllocator == nullptr,
		ZeroException<GpuCommandAllocator>({ "allocator" })
	);
}

CodeRed::GpuFrameBuffer::GpuFrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& render_target,
	const std::shared_ptr<GpuTexture>& depth_stencil) :
	mDevice(device),
	mRenderTargets({ render_target }),
	mDepthStencil(depth_stencil)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
	
	CODE_RED_DEBUG_THROW_IF(
		mRenderTargets[0] != nullptr && 
		mRenderTargets[0]->dimension() != Dimension::Dimension2D,
		InvalidException<GpuTexture>({ "render_target->dimension()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mRenderTargets[0] != nullptr && 
		!enumHas(mRenderTargets[0]->usage(), ResourceUsage::RenderTarget),
		InvalidException<GpuTexture>({ "render_target->usage()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mDepthStencil != nullptr && 
		mDepthStencil->dimension() != Dimension::Dimension2D,
		InvalidException<GpuTexture>({ "depth_stencil->dimension()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mDepthStencil != nullptr && 
		!enumHas(mDepthStencil->usage(), ResourceUsage::DepthStencil),
		InvalidException<GpuTexture>({ "depth_stencil->usage()" })
	);
}

CodeRed::GpuFence::GpuFence(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	mDevice(device)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuCommandQueue::GpuCommandQueue(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	mDevice(device)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuCommandAllocator::GpuCommandAllocator(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	mDevice(device)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuTexture::GpuTexture(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const ResourceInfo& info) :
	GpuResource(device, info)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		std::get<TextureProperty>(mInfo.Property).Size == 0,
		ZeroException<size_t>({ "info.Property.Size" })
	);
}

CodeRed::GpuSampler::GpuSampler(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const SamplerInfo& info) :
	mDevice(device),
	mInfo(info)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuBuffer::GpuBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const ResourceInfo& info) :
	GpuResource(device, info)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		mInfo.Type != ResourceType::Buffer,
		InvalidException<ResourceInfo>({ "info.Type" })
	);
	
	CODE_RED_DEBUG_THROW_IF(
		std::get<BufferProperty>(mInfo.Property).Size == 0,
		ZeroException<size_t>({ "info.Property.Size" })
	);

	//if we want to use the buffer as constant buffer,
	//the size of buffer need more than 256bytes
	//so we will check, warning and modify the size
	if (mInfo.Usage == ResourceUsage::ConstantBuffer) {
		
#ifdef __ENABLE__CODE__RED__DEBUG__
		if (std::get<BufferProperty>(mInfo.Property).Size < 256)
			DebugReport::warning("The size of constant buffer is less than 256bytes,"
				" we will create it with 256bytes.");
#endif

		//modify the size if it less than 256bytes
		std::get<BufferProperty>(mInfo.Property).Size = std::max(
			std::get<BufferProperty>(mInfo.Property).Size,
			static_cast<size_t>(256)
		);
	}
	
	CODE_RED_DEBUG_THROW_IF(
		enumHas(mInfo.Usage, ResourceUsage::RenderTarget) ||
		enumHas(mInfo.Usage, ResourceUsage::DepthStencil),
		InvalidException<ResourceUsage>({ "info.Usage" })
	);
}

CodeRed::GpuGraphicsPipeline::GpuGraphicsPipeline(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuResourceLayout>& resource_layout,
	const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
	const std::shared_ptr<GpuShaderState>& vertex_shader_state,
	const std::shared_ptr<GpuShaderState>& pixel_shader_state,
	const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
	const std::shared_ptr<GpuBlendState>& blend_state,
	const std::shared_ptr<GpuRasterizationState>& rasterization_state) :
	mRasterizationState(rasterization_state),
	mInputAssemblyState(input_assembly_state),
	mVertexShaderState(vertex_shader_state),
	mDepthStencilState(depth_stencil_state),
	mPixelShaderState(pixel_shader_state),
	mResourceLayout(resource_layout),
	mBlendState(blend_state),
	mRenderPass(render_pass),
	mDevice(device)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_PTR_VALID(mRasterizationState, "rasterization_state");
	CODE_RED_DEBUG_PTR_VALID(mInputAssemblyState, "input_assembly_state");
	CODE_RED_DEBUG_PTR_VALID(mVertexShaderState, "vertex_shader_state");
	CODE_RED_DEBUG_PTR_VALID(mDepthStencilState, "depth_stencil_state");
	CODE_RED_DEBUG_PTR_VALID(mPixelShaderState, "pixel_shader_state");
	CODE_RED_DEBUG_PTR_VALID(mResourceLayout, "resource_layout");
	CODE_RED_DEBUG_PTR_VALID(mBlendState, "blend_state");
	CODE_RED_DEBUG_PTR_VALID(mRenderPass, "render_pass");
}

CodeRed::GpuRenderPass::GpuRenderPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::optional<Attachment>& color,
	const std::optional<Attachment>& depth) :
	mDevice(device),
	mColorAttachments({ color }),
	mDepthAttachment(depth)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuDescriptorHeap::GpuDescriptorHeap(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuResourceLayout>& resource_layout) :
	mResourceLayout(resource_layout), mDevice(device)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
	
	CODE_RED_DEBUG_PTR_VALID(mResourceLayout, "resource_layout");
}

void CodeRed::GpuDescriptorHeap::bindResource(
	const size_t index, 
	const std::shared_ptr<GpuResource>& resource)
{
	if (resource->type() == ResourceType::Texture)
		bindTexture(index, std::static_pointer_cast<GpuTexture>(resource));
	else
		bindBuffer(index, std::static_pointer_cast<GpuBuffer>(resource));
}

void CodeRed::GpuRenderPass::setClear(
	const std::optional<ClearValue>& color,
	const std::optional<ClearValue>& depth)
{
	if (color.has_value()) mColor[0] = color.value();
	if (depth.has_value()) mDepth = depth.value();
}

void CodeRed::GpuGraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuTexture>& texture,
	const ResourceLayout layout)
{
	layoutTransition(texture, texture->layout(), layout);
}

void CodeRed::GpuGraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuBuffer>& buffer,
	const ResourceLayout layout)
{
	layoutTransition(buffer, buffer->layout(), layout);
}