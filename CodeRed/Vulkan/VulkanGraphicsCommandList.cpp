#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/FailedException.hpp"

#include "VulkanResource/VulkanTexture.hpp"
#include "VulkanResource/VulkanBuffer.hpp"

#include "VulkanGraphicsCommandList.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanCommandAllocator.hpp"
#include "VulkanResourceLayout.hpp"
#include "VulkanDescriptorHeap.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanRenderPass.hpp"

#undef min

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanGraphicsCommandList::VulkanGraphicsCommandList(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator) :
	GpuGraphicsCommandList(device, allocator)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto vkAllocator = std::static_pointer_cast<VulkanCommandAllocator>(mAllocator)->allocator();
	
	vk::CommandBufferAllocateInfo info = {};

	info
		.setPNext(nullptr)
		.setCommandPool(vkAllocator)
		.setLevel(vk::CommandBufferLevel::ePrimary)
		.setCommandBufferCount(1);

	mCommandBuffer = vkDevice.allocateCommandBuffers(info)[0];
}

CodeRed::VulkanGraphicsCommandList::~VulkanGraphicsCommandList()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto vkAllocator = std::static_pointer_cast<VulkanCommandAllocator>(mAllocator)->allocator();

	vkDevice.freeCommandBuffers(vkAllocator, mCommandBuffer);
}

void CodeRed::VulkanGraphicsCommandList::beginRecording()
{
	mCommandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

	mResourceLayout.reset();

	const vk::CommandBufferBeginInfo info = {};
	
	mCommandBuffer.begin(info);
}

void CodeRed::VulkanGraphicsCommandList::endRecording()
{
	mCommandBuffer.end();
}

void CodeRed::VulkanGraphicsCommandList::beginRenderPass(
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuFrameBuffer>& frame_buffer)
{
	CODE_RED_DEBUG_THROW_IF(
		mFrameBuffer != nullptr ||
		mRenderPass != nullptr,
		Exception("please end old render pass before you begin a new render pass.")
	)
	
	mRenderPass = std::static_pointer_cast<VulkanRenderPass>(render_pass);
	mFrameBuffer = std::static_pointer_cast<VulkanFrameBuffer>(frame_buffer);

	const auto clear = mRenderPass->getClear();

	vk::ClearValue clearValue[] = {
		vk::ClearColorValue(std::array<float, 4>({
			clear.first.Red,
			clear.first.Green,
			clear.first.Blue,
			clear.first.Alpha
		})),
		vk::ClearDepthStencilValue(clear.second.Depth, clear.second.Stencil)
	};
	
	vk::RenderPassBeginInfo info = {};

	info
		.setPNext(nullptr)
		.setClearValueCount(2)
		.setPClearValues(clearValue)
		.setRenderPass(mRenderPass->renderPass())
		.setFramebuffer(mFrameBuffer->frameBuffer())
		.setRenderArea(vk::Rect2D(
			vk::Offset2D(0, 0),
			vk::Extent2D(
				static_cast<uint32_t>(mFrameBuffer->frameBufferWidth()),
				static_cast<uint32_t>(mFrameBuffer->frameBufferHeight())
			)
		));

	tryLayoutTransition(mFrameBuffer->renderTarget(), mRenderPass->color(), false);
	tryLayoutTransition(mFrameBuffer->depthStencil(), mRenderPass->depth(), false);

	mCommandBuffer.beginRenderPass(info, vk::SubpassContents::eInline);
}

void CodeRed::VulkanGraphicsCommandList::endRenderPass()
{
	CODE_RED_DEBUG_THROW_IF(
		mFrameBuffer == nullptr ||
		mRenderPass == nullptr,
		Exception("please begin a render pass before end a render pass.")
	)
	
	mCommandBuffer.endRenderPass();

	tryLayoutTransition(mFrameBuffer->renderTarget(), mRenderPass->color(), true);
	tryLayoutTransition(mFrameBuffer->depthStencil(), mRenderPass->depth(), true);

	mFrameBuffer.reset();
	mRenderPass.reset();
}

void CodeRed::VulkanGraphicsCommandList::setGraphicsPipeline(
	const std::shared_ptr<GpuGraphicsPipeline>& pipeline)
{
	mCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
		std::static_pointer_cast<VulkanGraphicsPipeline>(pipeline)->pipeline());
}

void CodeRed::VulkanGraphicsCommandList::setResourceLayout(
	const std::shared_ptr<GpuResourceLayout>& layout)
{
	mResourceLayout = std::static_pointer_cast<VulkanResourceLayout>(layout);
}

void CodeRed::VulkanGraphicsCommandList::setVertexBuffer(
	const std::shared_ptr<GpuBuffer>& buffer)
{
	const auto vkBuffer = std::static_pointer_cast<VulkanBuffer>(buffer);
	
	mCommandBuffer.bindVertexBuffers(0, vkBuffer->buffer(), { 0 });
}

void CodeRed::VulkanGraphicsCommandList::setIndexBuffer(
	const std::shared_ptr<GpuBuffer>& buffer)
{
	const auto vkBuffer = std::static_pointer_cast<VulkanBuffer>(buffer);

	mCommandBuffer.bindIndexBuffer(vkBuffer->buffer(), 0, vk::IndexType::eUint32);
}

void CodeRed::VulkanGraphicsCommandList::setDescriptorHeap(
	const std::shared_ptr<GpuDescriptorHeap>& heap)
{
	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout == nullptr,
		InvalidException<GpuResourceLayout>({ "resource layout" })
	);
	
	CODE_RED_DEBUG_THROW_IF(
		heap->layout() != mResourceLayout,
		FailedException(DebugType::Set,
			{ "GpuDescriptorHeap", "Graphics Pipeline" },
			{ "current resource layout is not the one that create the heap." });
	);

	const auto vkHeap = std::static_pointer_cast<VulkanDescriptorHeap>(heap);

	CODE_RED_TRY_EXECUTE(
		heap->count() != 0,
		mCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
			mResourceLayout->layout(), 0, vkHeap->descriptorSets(), {})
	);
}

void CodeRed::VulkanGraphicsCommandList::setConstant32Bits(
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
				{ "Constant32Bits", "Graphics Pipeline" },
			{
				"the size of values is greater than the count of Constant32Bits."
				"We will only set values[0] to values[count - 1]."
			})
		);

	const auto constnat32Bits = mResourceLayout->constant32Bits();

	mCommandBuffer.pushConstants(
		mResourceLayout->layout(),
		enumConvert(constnat32Bits->Visibility),
		0,
		static_cast<uint32_t>(std::min(values.size(), constnat32Bits->Count) * sizeof(UInt32)),
		values.data()
	);
}

void CodeRed::VulkanGraphicsCommandList::setViewPort(const ViewPort& view_port)
{
	mCommandBuffer.setViewport(
		0, vk::Viewport(
			view_port.X,
			view_port.Y,
			view_port.Width,
			view_port.Height,
			view_port.MinDepth,
			view_port.MaxDepth
		)
	);
}

void CodeRed::VulkanGraphicsCommandList::setScissorRect(
	const ScissorRect& rect)
{
	mCommandBuffer.setScissor(0, vk::Rect2D(
		vk::Offset2D(
			static_cast<int32_t>(rect.Left),
			static_cast<int32_t>(rect.Top)),
		vk::Extent2D(
			static_cast<int32_t>(rect.Right), 
			static_cast<int32_t>(rect.Bottom))
	));
}

void CodeRed::VulkanGraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuTexture>& texture,
	const ResourceLayout old_layout, 
	const ResourceLayout new_layout)
{
	CODE_RED_DEBUG_THROW_IF(
		texture->layout() != old_layout,
		InvalidException<ResourceLayout>({ "old_layout" })
	);
	
	vk::ImageMemoryBarrier barrier = {};

	barrier
		.setPNext(nullptr)
		.setSrcAccessMask(enumConvert1(old_layout, ResourceType::Texture))
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setOldLayout(vk::ImageLayout::eUndefined)
		.setDstAccessMask(enumConvert1(new_layout, ResourceType::Texture))
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setNewLayout(enumConvert(new_layout))
		.setImage(std::static_pointer_cast<VulkanTexture>(texture)->image())
		.setSubresourceRange(
			vk::ImageSubresourceRange(
				enumConvert(texture->format(), texture->usage()),
				0,1,0,1
			));

	mCommandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eAllCommands,
		vk::PipelineStageFlagBits::eAllCommands,
		vk::DependencyFlags(0),
		{}, {},
		barrier);

	texture->setLayout(new_layout);
}

void CodeRed::VulkanGraphicsCommandList::layoutTransition(
	const std::shared_ptr<GpuBuffer>& buffer,
	const ResourceLayout old_layout, 
	const ResourceLayout new_layout)
{
	CODE_RED_DEBUG_THROW_IF(
		buffer->layout() != old_layout,
		InvalidException<ResourceLayout>({ "old_layout" })
	);

	vk::BufferMemoryBarrier barrier = {};

	barrier
		.setPNext(nullptr)
		.setSrcAccessMask(enumConvert1(old_layout, ResourceType::Buffer))
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstAccessMask(enumConvert1(new_layout, ResourceType::Buffer))
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(std::static_pointer_cast<VulkanBuffer>(buffer)->buffer())
		.setOffset(0)
		.setSize(buffer->size());

	mCommandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eAllCommands,
		vk::PipelineStageFlagBits::eAllCommands,
		vk::DependencyFlags(0),
		{}, barrier, {});

	buffer->setLayout(new_layout);
}

void CodeRed::VulkanGraphicsCommandList::copyBuffer(
	const std::shared_ptr<GpuBuffer>& source,
	const std::shared_ptr<GpuBuffer>& destination, 
	const size_t size, 
	const size_t source_offset,
	const size_t destination_offset)
{
	const vk::BufferCopy copy = {
		source_offset, destination_offset, static_cast<vk::DeviceSize>(size)
	};

	mCommandBuffer.copyBuffer(
		std::static_pointer_cast<VulkanBuffer>(source)->buffer(),
		std::static_pointer_cast<VulkanBuffer>(destination)->buffer(),
		copy);
}

void CodeRed::VulkanGraphicsCommandList::copyTexture(
	const std::shared_ptr<GpuTexture>& source,
	const std::shared_ptr<GpuTexture>& destination,
	const Extent3D<UInt32>& region,
	const size_t x,
	const size_t y,
	const size_t z)
{
	vk::ImageCopy copy = {};

	const auto size = vk::Extent3D(
		static_cast<uint32_t>(region.Right - region.Left),
		static_cast<uint32_t>(region.Bottom - region.Top),
		static_cast<uint32_t>(region.Back - region.Front)
	);

	copy
		.setExtent(size)
		.setSrcOffset(vk::Offset3D(
			static_cast<int32_t>(region.Left),
			static_cast<int32_t>(region.Top),
			static_cast<int32_t>(region.Front)))
		.setSrcSubresource(vk::ImageSubresourceLayers(
			enumHas(source->usage(), ResourceUsage::DepthStencil) ?
			vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil :
			vk::ImageAspectFlagBits::eColor,
			0, 0, 1))
		.setDstOffset(vk::Offset3D(
			static_cast<int32_t>(x),
			static_cast<int32_t>(y),
			static_cast<int32_t>(z)))
		.setDstSubresource(vk::ImageSubresourceLayers(
			enumHas(destination->usage(), ResourceUsage::DepthStencil) ?
			vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil :
			vk::ImageAspectFlagBits::eColor,
			0, 0, 1));

	mCommandBuffer.copyImage(
		std::static_pointer_cast<VulkanTexture>(source)->image(),
		enumConvert(source->layout()),
		std::static_pointer_cast<VulkanTexture>(destination)->image(),
		enumConvert(destination->layout()),
		copy
	);
}

void CodeRed::VulkanGraphicsCommandList::draw(
	const size_t vertex_count, 
	const size_t instance_count,
	const size_t start_vertex_location, 
	const size_t start_instance_location)
{
	mCommandBuffer.draw(
		static_cast<uint32_t>(vertex_count),
		static_cast<uint32_t>(instance_count),
		static_cast<uint32_t>(start_vertex_location),
		static_cast<uint32_t>(start_instance_location));
}

void CodeRed::VulkanGraphicsCommandList::drawIndexed(
	const size_t index_count, 
	const size_t instance_count,
	const size_t start_index_location, 
	const size_t base_vertex_location, 
	const size_t start_instance_location)
{
	mCommandBuffer.drawIndexed(
		static_cast<uint32_t>(index_count),
		static_cast<uint32_t>(instance_count),
		static_cast<uint32_t>(start_index_location),
		static_cast<uint32_t>(base_vertex_location),
		static_cast<uint32_t>(start_instance_location)
	);
}

void CodeRed::VulkanGraphicsCommandList::tryLayoutTransition(
	const std::shared_ptr<GpuTexture>& texture,
	const std::optional<Attachment>& attachment, 
	const bool final)
{
	CODE_RED_TRY_EXECUTE(
		texture != nullptr && attachment.has_value(),
		layoutTransition(texture, texture->layout(),
			final ? attachment->FinalLayout : attachment->InitialLayout)
	);
}

#endif
