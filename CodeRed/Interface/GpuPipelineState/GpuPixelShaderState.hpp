#pragma once

#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/Utility.hpp>

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