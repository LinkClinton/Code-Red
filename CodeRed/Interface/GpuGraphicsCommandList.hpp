#pragma once

#include "../Shared/Noncopyable.hpp"
#include "../Shared/ScissorRect.hpp"
#include "../Shared/ViewPort.hpp"

#include <memory>

namespace CodeRed {

	class GpuGraphicsPipeline;
	class GpuLogicalDevice;
	class GpuResourceLayout;
	class GpuFrameBuffer;
	class GpuTexture;
	class GpuBuffer;
	
	class GpuGraphicsCommandList : public Noncopyable {
	protected:
		explicit GpuGraphicsCommandList(const std::shared_ptr<GpuLogicalDevice>& device) :
			mDevice(device) {}
		
		~GpuGraphicsCommandList() = default;
	public:
		virtual void beginRecoding() = 0;

		virtual void endRecoding() = 0;

		virtual void setGraphicsPipeline(
			const std::shared_ptr<GpuGraphicsPipeline>& graphics_pipeline) = 0;
		
		virtual void setResourceLayout(
			const std::shared_ptr<GpuResourceLayout>& resource_layout) = 0;

		virtual void setVertexBuffer(
			const std::shared_ptr<GpuBuffer>& vertex_buffer) = 0;

		virtual void setGraphicsConstantBuffer(
			const size_t index,
			const std::shared_ptr<GpuBuffer>& buffer) = 0;

		virtual void setGraphicsTexture(
			const size_t index,
			const std::shared_ptr<GpuTexture> &texture) = 0;

		virtual void setFrameBuffer(
			const std::shared_ptr<GpuFrameBuffer>& frame_buffer) = 0;

		virtual void setViewPort(
			const ViewPort& view_port) = 0;

		virtual void setScissorRect(
			const ScissorRect& scissor_rect) = 0;

		virtual void clearRenderTarget(
			const std::shared_ptr<GpuFrameBuffer>& frame_buffer,
			const Real color[4],
			const size_t index = 0) = 0;

		virtual void clearDepthStencil(
			const std::shared_ptr<GpuFrameBuffer>& frame_buffer,
			const Real depth = 0,
			const UInt8 stencil = 0) = 0;

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
	};
	
}