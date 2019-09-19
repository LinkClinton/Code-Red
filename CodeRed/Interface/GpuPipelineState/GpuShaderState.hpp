#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/Utility.hpp>
#else
#include "GpuPipelineState.hpp"
#include "../../Shared/Utility.hpp"
#endif

#include <vector>

namespace CodeRed {

	class GpuShaderState : public GpuPipelineState {
	protected:
		GpuShaderState() = default;

		explicit GpuShaderState(
			const std::vector<Byte>& code) :
			mCode(code) {}
	public:
		auto code() const noexcept -> std::vector<Byte> { return mCode; }
	protected:
		std::vector<Byte> mCode = {};
	};
	
}