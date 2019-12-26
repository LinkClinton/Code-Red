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

		explicit GpuFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTextureRef>& render_target,
			const std::shared_ptr<GpuTextureRef>& depth_stencil = nullptr);

		~GpuFrameBuffer() = default;
	public:
		virtual void reset(
			const std::shared_ptr<GpuTextureRef>& render_target,
			const std::shared_ptr<GpuTextureRef>& depth_stencil) = 0;

		void reset(
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil) {
			reset(render_target->reference(), depth_stencil->reference());
		}
		
		auto renderTarget(const size_t index = 0) const -> std::shared_ptr<GpuTextureRef> { return mRenderTargets[index]; }

		auto depthStencil() const -> std::shared_ptr<GpuTextureRef> { return mDepthStencil; }

		auto fullViewPort(const size_t index = 0) const noexcept -> ViewPort;

		auto fullScissorRect(const size_t index = 0) const noexcept -> ScissorRect;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTextureRef>> mRenderTargets;
		std::shared_ptr<GpuTextureRef> mDepthStencil;
	};
	
}