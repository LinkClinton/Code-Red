#pragma once

#include "../../Shared/Utility.hpp"
#include "GpuPipelineState.hpp"

#include <vector>

namespace CodeRed {

	class GpuVertexShaderState : public GpuPipelineState {
	protected:
		GpuVertexShaderState() = default;

		explicit GpuVertexShaderState(
			const std::vector<Byte>& code) :
			mCode(code) {}
	public:
		auto code() const noexcept -> const std::vector<Byte> & { return mCode; }
	protected:
		std::vector<Byte> mCode = {};
	};
	
}