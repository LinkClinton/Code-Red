#include "VulkanGraphicsCommandList.hpp"
#include "VulkanCommandAllocator.hpp"
#include "VulkanLogicalDevice.hpp"

#ifdef __ENABLE__VULKAN__

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

void CodeRed::VulkanGraphicsCommandList::beginRecoding()
{
	mCommandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
	mCommandBuffer.begin({});
	
	mResourceLayout.reset();
}

void CodeRed::VulkanGraphicsCommandList::endRecoding() {
}

void CodeRed::VulkanGraphicsCommandList::setGraphicsPipeline(const std::shared_ptr<GpuGraphicsPipeline>& pipeline) {
}

void CodeRed::VulkanGraphicsCommandList::setResourceLayout(const std::shared_ptr<GpuResourceLayout>& layout) {
}

void CodeRed::VulkanGraphicsCommandList::setVertexBuffer(const std::shared_ptr<GpuBuffer>& buffer) {
}

void CodeRed::VulkanGraphicsCommandList::setIndexBuffer(const std::shared_ptr<GpuBuffer>& buffer) {
}

void CodeRed::VulkanGraphicsCommandList::setGraphicsConstantBuffer(const size_t index,
	const std::shared_ptr<GpuBuffer>& buffer) {
}

void CodeRed::VulkanGraphicsCommandList::setGraphicsTexture(const size_t index,
	const std::shared_ptr<GpuTexture>& texture) {
}

void CodeRed::VulkanGraphicsCommandList::setFrameBuffer(const std::shared_ptr<GpuFrameBuffer>& buffer) {
}

void CodeRed::VulkanGraphicsCommandList::setViewPort(const ViewPort& view_port) {
}

void CodeRed::VulkanGraphicsCommandList::setScissorRect(const ScissorRect& rect) {
}

void CodeRed::VulkanGraphicsCommandList::clearRenderTarget(const std::shared_ptr<GpuFrameBuffer>& buffer,
	const Real color[4], const size_t index) {
}

void CodeRed::VulkanGraphicsCommandList::clearDepthStencil(const std::shared_ptr<GpuFrameBuffer>& buffer,
	const Real depth, const UInt8 stencil) {
}

void CodeRed::VulkanGraphicsCommandList::layoutTransition(const std::shared_ptr<GpuTexture>& texture,
	const ResourceLayout old_layout, const ResourceLayout new_layout) {
}

void CodeRed::VulkanGraphicsCommandList::layoutTransition(const std::shared_ptr<GpuBuffer>& buffer,
	const ResourceLayout old_layout, const ResourceLayout new_layout) {
}

void CodeRed::VulkanGraphicsCommandList::copyBuffer(const std::shared_ptr<GpuBuffer>& source,
	const std::shared_ptr<GpuBuffer>& destination, const size_t size, const size_t source_offset,
	const size_t destination_offset) {
}

void CodeRed::VulkanGraphicsCommandList::copyTexture(const std::shared_ptr<GpuTexture>& source,
	const std::shared_ptr<GpuTexture>& destination, const Extent3D<UInt32>& region, const size_t x, const size_t y,
	const size_t z) {
}

void CodeRed::VulkanGraphicsCommandList::draw(const size_t vertex_count, const size_t instance_count,
	const size_t start_vertex_location, const size_t start_instance_location) {
}

void CodeRed::VulkanGraphicsCommandList::drawIndexed(const size_t index_count, const size_t instance_count,
	const size_t start_index_location, const size_t base_vertex_location, const size_t start_instance_location) {
}

#endif
