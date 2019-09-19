#include "../Shared/Exception/ZeroException.hpp"

#include "GpuResource/GpuTexture.hpp"
#include "GpuResource/GpuSampler.hpp"
#include "GpuResource/GpuBuffer.hpp"

#include "GpuGraphicsCommandList.hpp"
#include "GpuCommandAllocator.hpp"
#include "GpuGraphicsPipeline.hpp"
#include "GpuResourceLayout.hpp"
#include "GpuLogicalDevice.hpp"
#include "GpuCommandQueue.hpp"
#include "GpuFrameBuffer.hpp"
#include "GpuSwapChain.hpp"
#include "GpuFence.hpp"

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
	const WindowInfo& info,
	const PixelFormat& format, 
	const size_t buffer_count) :
	mDevice(device),
	mBuffers(buffer_count),
	mInfo(info),
	mPixelFormat(format)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
	
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
		mRenderTargets[0] == nullptr,
		ZeroException<GpuTexture>({ "render_target" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mRenderTargets[0]->dimension() != Dimension::Dimension2D,
		InvalidException<GpuTexture>({ "render_target->dimension()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		!enumHas(mRenderTargets[0]->usage(), ResourceUsage::RenderTarget),
		InvalidException<GpuTexture>({ "render_target->usage()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mDepthStencil != nullptr && mDepthStencil->dimension() != Dimension::Dimension2D,
		InvalidException<GpuTexture>({ "depth_stencil->dimension()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mDepthStencil != nullptr && !enumHas(mDepthStencil->usage(), ResourceUsage::DepthStencil),
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
		std::get<BufferProperty>(mInfo.Property).Size == 0,
		ZeroException<size_t>({ "info.Property.Size" })
	);

	CODE_RED_DEBUG_THROW_IF(
		enumHas(mInfo.Usage, ResourceUsage::RenderTarget) ||
		enumHas(mInfo.Usage, ResourceUsage::DepthStencil),
		InvalidException<ResourceUsage>({ "info.Usage" })
	);
}

CodeRed::GpuGraphicsPipeline::GpuGraphicsPipeline(
	const std::shared_ptr<GpuLogicalDevice>& device,
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
}