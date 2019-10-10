#pragma once

#include "../Shared/Enum/ResourceLayout.hpp"
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

		virtual void setIndexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer) = 0;

		virtual void setDescriptorHeap(
			const std::shared_ptr<GpuDescriptorHeap>& heap) = 0;

		virtual void setConstant32Bits(
			const std::vector<Value32Bit>& values) = 0;
		
		virtual void setViewPort(
			const ViewPort& view_port) = 0;

		virtual void setScissorRect(
			const ScissorRect& rect) = 0;

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
			const Extent3D<UInt32> &region,
			const size_t x = 0,
			const size_t y = 0,
			const size_t z = 0) = 0;
		
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