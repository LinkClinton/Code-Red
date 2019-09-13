#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

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
			mRenderTargets({ render_target }),
			mDepthStencil(depth_stencil) {}

		~GpuFrameBuffer() = default;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTexture>> mRenderTargets;
		std::shared_ptr<GpuTexture> mDepthStencil;
	};
	
}