#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuTexture;
	
	class GpuFrameBuffer : public Noncopyable {
	public:
		GpuFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil) :
			mDevice(device),
			mRenderTarget(render_target),
			mDepthStencil(depth_stencil) {}
	
		~GpuFrameBuffer() = default;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::shared_ptr<GpuTexture> mRenderTarget;
		std::shared_ptr<GpuTexture> mDepthStencil;
	};
	
}