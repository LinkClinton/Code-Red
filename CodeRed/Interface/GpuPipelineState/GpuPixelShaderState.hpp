#pragma once

#include "../../Shared/Utility.hpp"
#include "GpuPipelineState.hpp"

#include <vector>

namespace CodeRed {

	class GpuPixelShaderState : public GpuPipelineState {
	protected:
		GpuPixelShaderState() = default;

		explicit GpuPixelShaderState(
			const std::vector<Byte>& code) :
			mCode(code) {}
	public:
		auto code() const noexcept -> std::vector<Byte> { return mCode; }
	protected:
		std::vector<Byte> mCode = {};
	};
	
}