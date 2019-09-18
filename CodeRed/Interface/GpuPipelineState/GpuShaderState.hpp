#pragma once

#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/Utility.hpp>

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