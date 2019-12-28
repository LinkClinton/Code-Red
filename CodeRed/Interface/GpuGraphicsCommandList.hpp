#pragma once

#include "../Shared/Information/TextureBufferCopyInfo.hpp"
#include "../Shared/Information/TextureCopyInfo.hpp"
#include "../Shared/Enum/ResourceLayout.hpp"
#include "../Shared/Enum/IndexType.hpp"
#include "../Shared/Constant32Bits.hpp"
#include "../Shared/Noncopyable.hpp"
#include "../Shared/ScissorRect.hpp"
#include "../Shared/ViewPort.hpp"
#include "../Shared/Extent.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuCommandAllocator;
	class GpuGraphicsPipeline;
	class GpuResourceLayout;
	class GpuDescriptorHeap;
	class GpuLogicalDevice;	
	class GpuFrameBuffer;
	class GpuRenderPass;

	class GpuTextureBuffer;
	class GpuSampler;
	class GpuTexture;
	class GpuBuffer;
	
	class GpuGraphicsCommandList : public Noncopyable {
	protected:
		explicit GpuGraphicsCommandList(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator);
		
		~GpuGraphicsCommandList() = default;
	public:
		virtual void beginRecording() = 0;

		virtual void endRecording() = 0;

		virtual void beginRenderPass(
			const std::shared_ptr<GpuRenderPass> &render_pass,
			const std::shared_ptr<GpuFrameBuffer> &frame_buffer) = 0;

		virtual void endRenderPass() = 0;
		
		virtual void setGraphicsPipeline(
			const std::shared_ptr<GpuGraphicsPipeline>& pipeline) = 0;
		
		virtual void setResourceLayout(
			const std::shared_ptr<GpuResourceLayout>& layout) = 0;

		virtual void setVertexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer) = 0;

		virtual void setVertexBuffers(
			const std::vector<std::shared_ptr<GpuBuffer>>& buffers,
			const size_t startSlot = 0) = 0;

		virtual void setIndexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer,
			const IndexType type = IndexType::UInt32) = 0;

		virtual void setDescriptorHeap(
			const std::shared_ptr<GpuDescriptorHeap>& heap) = 0;

		virtual void setConstant32Bits(
			const std::vector<Value32Bit>& values) = 0;
		
		virtual void setViewPort(
			const ViewPort& view_port) = 0;

		virtual void setScissorRect(
			const ScissorRect& rect) = 0;

		virtual void layoutTransition(
			const std::shared_ptr<GpuTextureBuffer>& buffer,
			const ResourceLayout old_layout,
			const ResourceLayout new_layout) = 0;

		virtual void layoutTransition(
			const std::shared_ptr<GpuTextureBuffer>& buffer,
			const ResourceLayout layout);
		
		virtual void layoutTransition(
			const std::shared_ptr<GpuTexture>& texture,
			const ResourceLayout old_layout,
			const ResourceLayout new_layout) = 0;

		virtual void layoutTransition(
			const std::shared_ptr<GpuTexture>& texture,
			const ResourceLayout layout);

		virtual void layoutTransition(
			const std::shared_ptr<GpuBuffer>& buffer,
			const ResourceLayout old_layout,
			const ResourceLayout new_layout) = 0;

		virtual void layoutTransition(
			const std::shared_ptr<GpuBuffer>& buffer,
			const ResourceLayout layout);
		
		virtual void copyBuffer(
			const std::shared_ptr<GpuBuffer>& source,
			const std::shared_ptr<GpuBuffer>& destination,
			const size_t size,
			const size_t source_offset = 0,
			const size_t destination_offset = 0) = 0;

		virtual void copyTexture(
			const std::shared_ptr<GpuTexture>& source,
			const std::shared_ptr<GpuTexture>& destination,
			const Extent3D<size_t> &region,
			const size_t x = 0,
			const size_t y = 0,
			const size_t z = 0) = 0;

		virtual void copyTexture(
			const TextureCopyInfo& source,
			const TextureCopyInfo& destination,
			const size_t width,
			const size_t height,
			const size_t depth = 1) = 0;

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
		
		virtual void copyMemoryToBuffer(
			const std::shared_ptr<GpuBuffer>& destination,
			const void* data) = 0;
		
		virtual void copyMemoryToTexture(
			const std::shared_ptr<GpuTexture>& destination,
			const void* data) = 0;

		virtual void copyMemoryToTexture(
			const std::shared_ptr<GpuTexture>& destination,
			const size_t resourceIndex,
			const void* data) = 0;
		
		virtual void draw(
			const size_t vertex_count,
			const size_t instance_count = 1,
			const size_t start_vertex_location = 0,
			const size_t start_instance_location = 0) = 0;

		virtual void drawIndexed(
			const size_t index_count,
			const size_t instance_count = 1,
			const size_t start_index_location = 0,
			const size_t base_vertex_location = 0,
			const size_t start_instance_location = 0) = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		std::shared_ptr<GpuCommandAllocator> mAllocator;
	};
	
}