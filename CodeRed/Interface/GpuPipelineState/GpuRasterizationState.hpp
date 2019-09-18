#pragma once

#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/Enum/PixelFormat.hpp>
#include <Shared/Enum/FrontFace.hpp>
#include <Shared/Enum/CullMode.hpp>
#include <Shared/Enum/FillMode.hpp>

#include <vector>

namespace CodeRed {

	class GpuRasterizationState : GpuPipelineState {
	protected:
		GpuRasterizationState() = default;

		explicit GpuRasterizationState(
			const PixelFormat render_target_format,
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true) :
			mRenderTargetFormats({ render_target_format }),
			mFrontFace(front_face),
			mCullMode(cull_mode),
			mFillMode(fill_mode),
			mDepthClamp(depth_clamp) {}
	public:
		auto renderTargetFormat(const size_t index = 0) const -> PixelFormat { return mRenderTargetFormats[index]; }
		
		auto frontFace() const noexcept -> FrontFace { return mFrontFace; }

		auto cullMode() const noexcept -> CullMode { return mCullMode; }

		auto fillMode() const noexcept -> FillMode { return mFillMode; }

		bool depthClamp() const noexcept { return mDepthClamp; }
	protected:
		std::vector<PixelFormat> mRenderTargetFormats = { PixelFormat::Unknown };
		
		FrontFace mFrontFace = FrontFace::Clockwise;
		CullMode mCullMode = CullMode::Back;
		FillMode mFillMode = FillMode::Solid;

		bool mDepthClamp = true;
	};
	
}