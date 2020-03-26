#pragma once

#include "../Interface/GpuGraphicsCommandList.hpp"
#include "../Shared/Attachment.hpp"
#include "VulkanUtility.hpp"

#include <optional>

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanResourceLayout;
	class VulkanFrameBuffer;
	class VulkanRenderPass;

	class GpuTextureRef;
	
	class VulkanGraphicsCommandList final : public GpuGraphicsCommandList {
	public:
		explicit VulkanGraphicsCommandList(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator);

		~VulkanGraphicsCommandList();

		void beginRecording() override;

		void endRecording() override;

		void beginRenderPass(
			const std::shared_ptr<GpuRenderPass>& render_pass, 
			const std::shared_ptr<GpuFrameBuffer>& frame_buffer) override;

		void endRenderPass() override;
		
		void setGraphicsPipeline(
			const std::shared_ptr<GpuGraphicsPipeline>& pipeline) override;

		void setResourceLayout(
			const std::shared_ptr<GpuResourceLayout>& layout) override;

		void setVertexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer) override;

		void setVertexBuffers(
			const std::vector<std::shared_ptr<GpuBuffer>>& buffers, 
			const size_t startSlot) override;
		
		void setIndexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer,
			const IndexType type) override;

		void setDescriptorHeap(
			const std::shared_ptr<GpuDescriptorHeap>& heap) override;

		void setConstant32Bits(
			const std::vector<Value32Bit>& values) override;
		
		void setViewPort(
			const ViewPort& view_port) override;

		void setScissorRect(
			const ScissorRect& rect) override;

		void layoutTransition(
			const std::shared_ptr<GpuTextureBuffer>& buffer, 
			const ResourceLayout old_layout, 
			const ResourceLayout new_layout) override;
		
		void layoutTransition(
			const std::shared_ptr<GpuTexture>& texture,
			const ResourceLayout old_layout,
			const ResourceLayout new_layout) override;

		void layoutTransition(
			const std::shared_ptr<GpuBuffer>& buffer,
			const ResourceLayout old_layout,
			const ResourceLayout new_layout) override;
		
		void resolveTexture(
			const TextureResolveInfo& source, 
			const TextureResolveInfo& destination) override;
		
		void copyBuffer(
			const std::shared_ptr<GpuBuffer>& source,
			const std::shared_ptr<GpuBuffer>& destination,
			const size_t size,
			const size_t source_offset = 0,
			const size_t destination_offset = 0) override;

		void copyTexture(
			const std::shared_ptr<GpuTexture>& source,
			const std::shared_ptr<GpuTexture>& destination,
			const Extent3D<size_t>& region,
			const size_t x = 0,
			const size_t y = 0,
			const size_t z = 0) override;

		void copyTexture(
			const TextureCopyInfo& source, 
			const TextureCopyInfo& destination, 
			const size_t width, 
			const size_t height, 
			const size_t depth) override;

		void copyTextureToBuffer(
			const TextureCopyInfo& source, 
			const TextureBufferCopyInfo& destination, 
			const size_t width, 
			const size_t height, 
			const size_t depth) override;

		void copyBufferToTexture(
			const TextureBufferCopyInfo& source, 
			const TextureCopyInfo& destination, 
			const size_t width, 
			const size_t height, 
			const size_t depth) override;
		
		void draw(
			const size_t vertex_count,
			const size_t instance_count = 1,
			const size_t start_vertex_location = 0,
			const size_t start_instance_location = 0) override;

		void drawIndexed(
			const size_t index_count,
			const size_t instance_count,
			const size_t start_index_location,
			const size_t base_vertex_location,
			const size_t start_instance_location) override;

		auto commandList() const noexcept -> vk::CommandBuffer { return mCommandBuffer; }
	private:
		void tryLayoutTransition(
			const std::shared_ptr<GpuTextureRef>& texture,
			const std::optional<Attachment>& attachment,
			const bool final = false);
	private:
		vk::CommandBuffer mCommandBuffer;
		
		std::shared_ptr<VulkanResourceLayout> mResourceLayout;
		std::shared_ptr<VulkanFrameBuffer> mFrameBuffer;
		std::shared_ptr<VulkanRenderPass> mRenderPass;
	};
	
}

#endif