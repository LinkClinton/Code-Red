#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/StencilOperatorInfo.hpp>
#else
#include "GpuPipelineState.hpp"
#include "../../Shared/StencilOperatorInfo.hpp"
#endif

namespace CodeRed {

	class GpuDepthStencilState : public GpuPipelineState {
	protected:
		explicit GpuDepthStencilState(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo()) :
			GpuPipelineState(device),
			mDepthWriteEnable(depth_write_enable),
			mStencilEnable(stencil_enable),
			mDepthEnable(depth_enable),
			mDepthOperator(depth_operator),
			mFrontFace(front),
			mBackFace(back) {}

		~GpuDepthStencilState() = default;
	public:
		bool depthWriteEnable() const noexcept { return mDepthWriteEnable; }

		bool stencilEnable() const noexcept { return mStencilEnable; }
		
		bool dpethEnable() const noexcept { return mDepthEnable; }

		auto depthOperator() const noexcept -> CompareOperator { return mDepthOperator; }

		auto frontFace() const noexcept { return mFrontFace; }

		auto backFace() const noexcept { return mBackFace; }
	protected:
		bool mDepthWriteEnable = true;
		bool mStencilEnable = false;
		bool mDepthEnable = true;
		
		CompareOperator mDepthOperator = CompareOperator::LessEqual;

		StencilOperatorInfo mFrontFace = StencilOperatorInfo();
		StencilOperatorInfo mBackFace = StencilOperatorInfo();
	};
	
}