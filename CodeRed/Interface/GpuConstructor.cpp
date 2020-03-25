#include "../Shared/Exception/ZeroException.hpp"

#include "GpuResource/GpuTextureBuffer.hpp"
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
#include "GpuTextureRef.hpp"
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
	//check the device, queue, info and buffer count
	//the device, queue, info.handle must be a valid value
	//and the info.width, info.height and buffer count can not be zero.
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
	const std::vector<SamplerLayoutElement>& samplers,
	const std::optional<Constant32Bits>& constant32Bits) :
	mDevice(device),
	mElements(elements),
	mSamplers(samplers),
	mConstant32Bits(constant32Bits)
{
	//the device must be a valid device
	//the size of elements and samplers can be zero
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);
}

CodeRed::GpuLogicalDevice::GpuLogicalDevice(
	const std::shared_ptr<GpuDisplayAdapter>& adapter,
	const APIVersion version) :
	mDisplayAdapter(adapter), mAPIVersion(version)
{
	//the display adapter must be valid
	//it must be create from GpuSystemInfo
	CODE_RED_DEBUG_THROW_IF(
		mDisplayAdapter == nullptr,
		ZeroException<GpuDisplayAdapter>({ "adapter" })
	);

	CODE_RED_DEBUG_THROW_IF(
		version == APIVersion::Unknown,
		InvalidException<APIVersion>({ "version" })
	);
}

CodeRed::GpuGraphicsCommandList::GpuGraphicsCommandList(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator) :
	mDevice(device),
	mAllocator(allocator)
{
	//the device and allocator must be valid.
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		mAllocator == nullptr,
		ZeroException<GpuCommandAllocator>({ "allocator" })
	);
}

CodeRed::GpuFrameBuffer::GpuFrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets,
	const std::shared_ptr<GpuTextureRef>& depth_stencil) :
	mDevice(device),
	mRenderTargets(render_targets),
	mDepthStencil(depth_stencil)
{
	//the device must be valid
	//but we can ignore the render target and depth stencil
	//if we create frame buffer without both, we will warning
	//the dimension of render target and depth stencil must be Dimension::2D
	//and the usage of render target must have ResourceUsage::RenderTarget
	//and the usage of depth stencil must have ResourceUsage::DepthStencil
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	for (size_t index = 0; index < mRenderTargets.size(); index++) {
		CODE_RED_DEBUG_THROW_IF(
			mRenderTargets[index] != nullptr &&
			mRenderTargets[index]->source()->dimension() != Dimension::Dimension2D,
			InvalidException<GpuTexture>({ "render_target->dimension()" })
		);

		CODE_RED_DEBUG_THROW_IF(
			mRenderTargets[index] != nullptr &&
			!enumHas(mRenderTargets[index]->source()->usage(), ResourceUsage::RenderTarget),
			InvalidException<GpuTexture>({ "render_target->usage()" })
		);
	}

	CODE_RED_DEBUG_THROW_IF(
		mDepthStencil != nullptr &&
		mDepthStencil->source()->dimension() != Dimension::Dimension2D,
		InvalidException<GpuTexture>({ "depth_stencil->dimension()" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mDepthStencil != nullptr &&
		!enumHas(mDepthStencil->source()->usage(), ResourceUsage::DepthStencil),
		InvalidException<GpuTexture>({ "depth_stencil->usage()" })
	);

	CODE_RED_DEBUG_TRY_EXECUTE(
		mRenderTargets.empty() &&
		mDepthStencil == nullptr,
		DebugReport::warning(DebugType::Create,
			{ "FrameBuffer" },
			{ "there are no rtv and dsv." })
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

CodeRed::GpuTextureBuffer::GpuTextureBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const TextureBufferInfo& info) : mDevice(device), mInfo(info)
{
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		mInfo.Size == 0,
		ZeroException<size_t>({ "info.Size" })
	);
}

CodeRed::GpuTexture::GpuTexture(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const ResourceInfo& info) :
	GpuResource(device, info)
{
	//the device must be a valid device.
	//and the size of texture must greater zero
	//and the info.Type must be ResourceType::Texture
	//and the info.Usage only support RT and DS(warning)
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		std::get<TextureProperty>(mInfo.Property).Size == 0,
		ZeroException<size_t>({ "info.Property.Size" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mInfo.Type != ResourceType::Texture,
		InvalidException<ResourceInfo>({ "info.Type" })
	);

	// we only support MSAA Texture with Dimension::Dimension2D
	CODE_RED_DEBUG_THROW_IF(
		std::get<TextureProperty>(mInfo.Property).Sample != MultiSample::Count1 &&
		std::get<TextureProperty>(mInfo.Property).Dimension != Dimension::Dimension2D,
		InvalidException<ResourceInfo>({ "info.Property.Dimension" },
			{ "We only support MSAA Texture with Dimension::Dimension2D." })
	);

	// we only support MSAA Texture with MipLevels 1
	CODE_RED_DEBUG_THROW_IF(
		std::get<TextureProperty>(mInfo.Property).Sample != MultiSample::Count1 &&
		std::get<TextureProperty>(mInfo.Property).MipLevels != 1,
		InvalidException<ResourceInfo>({ "info.Property.MipLevels" },
			{ "We only support MSAA Texture with MipLevels 1." })
	);

	// The MSAA Texture should has ResourceUsage::RenderTarget or ResourceUsage::DepthStencil
	CODE_RED_DEBUG_THROW_IF(
		std::get<TextureProperty>(mInfo.Property).Sample != MultiSample::Count1 &&
		!enumHas(mInfo.Usage, ResourceUsage::RenderTarget) &&
		!enumHas(mInfo.Usage, ResourceUsage::DepthStencil),
		InvalidException<ResourceInfo>({ "info.Usage" },
			{
				DebugReport::make("The MSAA Texture should has these usage [0] and [1].",
					{
						CODE_RED_TO_STRING(ResourceUsage::RenderTarget),
						CODE_RED_TO_STRING(ResourceUsage::DepthStencil)
					})
			})
	);

	CODE_RED_DEBUG_WARNING_IF(
		enumHas(mInfo.Usage, ResourceUsage::ConstantBuffer) ||
		enumHas(mInfo.Usage, ResourceUsage::IndexBuffer) ||
		enumHas(mInfo.Usage, ResourceUsage::VertexBuffer),
		DebugReport::make(
			"the usage of texture only support [0] and [1].",
			{
			CODE_RED_TO_STRING(ResourceUsage::RenderTarget),
			CODE_RED_TO_STRING(ResourceUsage::DepthStencil)
			}
		)
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
	//the device must be a valid device.
	//and the info.Type must be ResourceType::Buffer
	//and the info.Property.Size must greater than 0
	//if the buffer is constant buffer, the size must greater than 256bytes
	//and the usage of buffer only support Vertex, Index, Constant Buffer.
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_THROW_IF(
		mInfo.Type != ResourceType::Buffer &&
		mInfo.Type != ResourceType::GroupBuffer,
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
		
		CODE_RED_DEBUG_WARNING_IF(
			std::get<BufferProperty>(mInfo.Property).Size < 256,
			"The size of constant buffer is less than 256bytes,"
			" we will create it with 256bytes."
		);

		//modify the size if it less than 256bytes
		std::get<BufferProperty>(mInfo.Property).Size = std::max(
			std::get<BufferProperty>(mInfo.Property).Size,
			static_cast<size_t>(256)
		);
	}
	
	CODE_RED_DEBUG_WARNING_IF(
		enumHas(mInfo.Usage, ResourceUsage::RenderTarget) ||
		enumHas(mInfo.Usage, ResourceUsage::DepthStencil),
		DebugReport::make(
			"the usage of buffer only support [0], [1] and [2].",
			{
				CODE_RED_TO_STRING(ResourceUsage::VertexBuffer),
				CODE_RED_TO_STRING(ResourceUsage::IndexBuffer),
				CODE_RED_TO_STRING(ResourceUsage::ConstantBuffer)
			}
		)
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
	//all of them must be valid value.
	CODE_RED_DEBUG_DEVICE_VALID(mDevice);

	CODE_RED_DEBUG_PTR_VALID(mRasterizationState, "rasterization_state");
	CODE_RED_DEBUG_PTR_VALID(mInputAssemblyState, "input_assembly_state");
	CODE_RED_DEBUG_PTR_VALID(mVertexShaderState, "vertex_shader_state");
	CODE_RED_DEBUG_PTR_VALID(mDepthStencilState, "depth_stencil_state");
	CODE_RED_DEBUG_PTR_VALID(mPixelShaderState, "pixel_shader_state");
	CODE_RED_DEBUG_PTR_VALID(mResourceLayout, "resource_layout");
	CODE_RED_DEBUG_PTR_VALID(mBlendState, "blend_state");
	CODE_RED_DEBUG_PTR_VALID(mRenderPass, "render_pass");

	CODE_RED_DEBUG_THROW_IF(
		mVertexShaderState->type() != ShaderType::Vertex,
		InvalidException<GpuShaderState>({ "vertex_shader_state" }, { "the shader type is not vertex." })
	);

	CODE_RED_DEBUG_THROW_IF(
		mPixelShaderState->type() != ShaderType::Pixel,
		InvalidException<GpuShaderState>({ "pixel_shader_state" }, { "the shader type is not pixel." })
	);
}

CodeRed::GpuRenderPass::GpuRenderPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<Attachment>& colors, 
	const std::optional<Attachment>& depth) :
	mDevice(device),
	mColorAttachments(colors),
	mColors(colors.size()),
	mDepthAttachment(depth),
	mDepth(ClearValue())
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

	mCount = mResourceLayout->elements().size();
}

CodeRed::GpuTextureRef::GpuTextureRef(const std::shared_ptr<GpuTexture>& texture, const TextureRefInfo& info) :
	mTexture(texture), mInfo(info)
{
	// the MSAA texture is not support Cube Map
	CODE_RED_DEBUG_THROW_IF(
		mTexture->sample() != MultiSample::Count1 && mInfo.Usage == TextureRefUsage::CubeMap,
		InvalidException<TextureRefInfo>({ "info.Usage" },
			{ "The MSAA Texture is not support Cube Map." })
	);

	// check the range of mip levels and arrays
	// if the range is invalid, we will report warning and set it to default range
	CODE_RED_DEBUG_WARNING_IF(
		(mInfo.MipLevel.Start >= mInfo.MipLevel.End || mInfo.MipLevel.End > mTexture->mipLevels()) && 
		!(mInfo.MipLevel.Start == 0 && mInfo.MipLevel.End == 0),
		"The mip level of texture ref is invalid, we will use default range.");

	CODE_RED_DEBUG_WARNING_IF(
		(mInfo.Array.Start >= mInfo.Array.End || mInfo.Array.End > mTexture->arrays()) &&
		!(mInfo.Array.Start == 0 && mInfo.Array.End == 0),
		"The array of texture ref is invalid, we will use default range.");
	
	CODE_RED_TRY_EXECUTE(
		mInfo.MipLevel.Start >= mInfo.MipLevel.End || mInfo.MipLevel.End > mTexture->mipLevels(),
		mInfo.MipLevel = ValueRange<size_t>(0, mTexture->mipLevels())
	);

	CODE_RED_TRY_EXECUTE(
		mInfo.Array.Start >= mInfo.Array.End || mInfo.Array.End > mTexture->arrays(),
		mInfo.Array = ValueRange<size_t>(0, mTexture->arrays())
	);

	// check if the texture support the Cube Map
	// if the array of texture ref is less than 6, it can not be Cube Map.
	CODE_RED_DEBUG_WARNING_IF(
		mInfo.Array.size() < 6 && mInfo.Usage == TextureRefUsage::CubeMap,
		"The array of texture ref is less than 6, so we can not set the usage as Cube Map. We will set it to Common."
	);

	CODE_RED_TRY_EXECUTE(
		mInfo.Array.size() < 6 && mInfo.Usage == TextureRefUsage::CubeMap,
		mInfo.Usage = TextureRefUsage::Common
	);

}

void CodeRed::GpuDescriptorHeap::bindResource(
	const std::shared_ptr<GpuResource>& resource,
	const size_t index)
{
	switch (resource->type()) {
	case ResourceType::Buffer: bindBuffer(std::static_pointer_cast<GpuBuffer>(resource), index);
	case ResourceType::Texture: bindTexture(std::static_pointer_cast<GpuTexture>(resource), index);
	case ResourceType::GroupBuffer: bindBuffer(std::static_pointer_cast<GpuBuffer>(resource), index);
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

void CodeRed::GpuRenderPass::setClear(
	const std::optional<ClearValue>& color,
	const std::optional<ClearValue>& depth)
{
	mColors = color.has_value() ? std::vector<ClearValue>{color.value()} : std::vector<ClearValue>{};
	mDepth = depth;
}

void CodeRed::GpuRenderPass::setClear(
	const std::vector<ClearValue>& colors, 
	const std::optional<ClearValue>& depth)
{
	mColors = colors;
	mDepth = depth;
}

auto CodeRed::GpuRenderPass::compatible(const std::shared_ptr<GpuFrameBuffer>& frameBuffer) const -> bool
{
	// the number of color attachments should greater than the number of render targets
	// the depth attachment can not be null when the depth stencil is existed.
	if (mColorAttachments.size() < frameBuffer->size()) return false;
	if (!mDepthAttachment.has_value() && frameBuffer->depthStencil() != nullptr) return false;

	return true;
}

void CodeRed::GpuGraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuTextureBuffer>& buffer,
	const ResourceLayout layout)
{
	layoutTransition(buffer, buffer->layout(), layout);
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

auto CodeRed::GpuTexture::width(const size_t mipSlice) const noexcept -> size_t
{
	auto result = std::get<TextureProperty>(mInfo.Property).Width;
	
	for (size_t index = 0; index < mipSlice; index++) 
		result = std::max(result / 2, static_cast<size_t>(1));

	return result;
}

auto CodeRed::GpuTexture::height(const size_t mipSlice) const noexcept -> size_t
{
	auto result = std::get<TextureProperty>(mInfo.Property).Height;

	for (size_t index = 0; index < mipSlice; index++)
		result = std::max(result / 2, static_cast<size_t>(1));

	return result;
}

auto CodeRed::GpuTexture::depth(const size_t mipSlice) const noexcept -> size_t
{
	auto result = std::get<TextureProperty>(mInfo.Property).Depth;

	// if the texture is array, the depth is always the 1
	if (isArray()) return 1;
	
	for (size_t index = 0; index < mipSlice; index++)
		result = std::max(result / 2, static_cast<size_t>(1));

	return result;
}

auto CodeRed::GpuTexture::size(const size_t mipSlice) const noexcept -> size_t
{
	// the size of texture only for one texture with one mip level
	// so if the texture is array, we do not use depth
	// if the texture is MSAA texture, we will calculate the sample count.
	return width(mipSlice) * height(mipSlice) * depth(mipSlice) *
		PixelFormatSizeOf::get(format()) * MultiSampleSizeOf::get(sample());
}

auto CodeRed::GpuFrameBuffer::fullViewPort(const size_t index) const noexcept -> ViewPort
{
	return {
		0.0f, 0.0f,
		static_cast<Real>(mRenderTargets[index]->source()->width(mRenderTargets[index]->mipLevel().Start)),
		static_cast<Real>(mRenderTargets[index]->source()->height(mRenderTargets[index]->mipLevel().Start)),
		0.0f, 1.0f
	};
}

auto CodeRed::GpuFrameBuffer::fullScissorRect(const size_t index) const noexcept -> ScissorRect
{
	return {
		0, 0,
		mRenderTargets[index]->source()->width(mRenderTargets[index]->mipLevel().Start),
		mRenderTargets[index]->source()->height(mRenderTargets[index]->mipLevel().Start)
	};
}