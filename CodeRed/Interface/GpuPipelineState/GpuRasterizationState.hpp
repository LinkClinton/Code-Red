#pragma once

#include "GpuPipelineState.hpp"
#include "../../Shared/Enum/FrontFace.hpp"
#include "../../Shared/Enum/CullMode.hpp"
#include "../../Shared/Enum/FillMode.hpp"

#include <vector>

namespace CodeRed {

	class GpuRasterizationState : GpuPipelineState {
	protected:
		explicit GpuRasterizationState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true) :
			GpuPipelineState(device),
			mFrontFace(front_face),
			mCullMode(cull_mode),
			mFillMode(fill_mode),
			mDepthClamp(depth_clamp) {}

		~GpuRasterizationState() = default;
	public:
		auto frontFace() const noexcept -> FrontFace { return mFrontFace; }

		auto cullMode() const noexcept -> CullMode { return mCullMode; }

		auto fillMode() const noexcept -> FillMode { return mFillMode; }

		bool depthClamp() const noexcept { return mDepthClamp; }
	protected:
		FrontFace mFrontFace = FrontFace::Clockwise;
		CullMode mCullMode = CullMode::Back;
		FillMode mFillMode = FillMode::Solid;

		bool mDepthClamp = true;
	};
	
}