#pragma once

#include "../../Shared/StencilOperatorInfo.hpp"
#include "../../Shared/Enum/PixelFormat.hpp"
#include "GpuPipelineState.hpp"

namespace CodeRed {

	class GpuDepthStencilState : public GpuPipelineState {
	public:
		GpuDepthStencilState() = default;

		explicit GpuDepthStencilState(
			const PixelFormat depth_stencil_format,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo()) :
			mDepthWriteEnable(depth_write_enable),
			mStencilEnable(stencil_enable),
			mDepthEnable(depth_enable),
			mDepthOperator(depth_operator),
			mDepthStencilFormat(depth_stencil_format),
			mFrontFace(front),
			mBackFace(back) {}

		~GpuDepthStencilState() = default;
		
		bool depthWriteEnable() const noexcept { return mDepthWriteEnable; }

		bool stencilEnable() const noexcept { return mStencilEnable; }
		
		bool dpethEnable() const noexcept { return mDepthEnable; }

		auto depthOperator() const noexcept -> CompareOperator { return mDepthOperator; }

		auto depthStencilFormat() const noexcept -> PixelFormat { return mDepthStencilFormat; }
		
		auto frontFace() const noexcept { return mFrontFace; }

		auto backFace() const noexcept { return mBackFace; }
	protected:
		bool mDepthWriteEnable = true;
		bool mStencilEnable = false;
		bool mDepthEnable = true;
		
		CompareOperator mDepthOperator = CompareOperator::LessEqual;

		PixelFormat mDepthStencilFormat = PixelFormat::Unknown;
		
		StencilOperatorInfo mFrontFace = StencilOperatorInfo();
		StencilOperatorInfo mBackFace = StencilOperatorInfo();
	};
	
}