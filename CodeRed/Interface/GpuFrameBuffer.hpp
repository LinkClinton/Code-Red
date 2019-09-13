#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuTexture;
	
	class GpuFrameBuffer : public Noncopyable {
	protected:
		GpuFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil) :
			mDevice(device),
			mRenderTargets({ render_target }),
			mDepthStencil(depth_stencil) {}

		~GpuFrameBuffer() = default;
	public:
		auto renderTarget(const size_t index = 0) const -> std::shared_ptr<GpuTexture> { return mRenderTargets[index]; }

		auto depthStencil() const -> std::shared_ptr<GpuTexture> { return mDepthStencil; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTexture>> mRenderTargets;
		std::shared_ptr<GpuTexture> mDepthStencil;
	};
	
}