#pragma once

#include "GpuResource/GpuTexture.hpp"
#include "../Shared/Noncopyable.hpp"
#include "../Shared/ScissorRect.hpp"
#include "../Shared/ViewPort.hpp"

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

		auto fullViewPort(const size_t index = 0) const noexcept -> ViewPort {
			return {
				0,0,
				static_cast<Real>(mRenderTargets[index]->width()),
				static_cast<Real>(mRenderTargets[index]->height()),
				0.0f,1.0f
			};
		}

		auto fullScissorRect(const size_t index = 0) const noexcept -> ScissorRect {
			return {
				0, 0,
				static_cast<UInt32>(mRenderTargets[index]->width()),
				static_cast<UInt32>(mRenderTargets[index]->height())
			};
		}
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTexture>> mRenderTargets;
		std::shared_ptr<GpuTexture> mDepthStencil;
	};
	
}