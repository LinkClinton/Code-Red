#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#else
#include "../Shared/Noncopyable.hpp"
#endif

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuFrameBuffer : public Noncopyable {
	protected:
		explicit GpuFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil = nullptr);

		~GpuFrameBuffer() = default;
	public:
		virtual void reset(
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil) = 0;
		
		auto renderTarget(const size_t index = 0) const -> std::shared_ptr<GpuTexture> { return mRenderTargets[index]; }

		auto depthStencil() const -> std::shared_ptr<GpuTexture> { return mDepthStencil; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTexture>> mRenderTargets;
		std::shared_ptr<GpuTexture> mDepthStencil;
	};
	
}