#pragma once

#include "GpuPipelineState.hpp"
#include "../../Shared/Utility.hpp"
#include "../../Shared/Enum/ShaderType.hpp"

#include <vector>
#include <string>

namespace CodeRed {

	class GpuShaderState : public GpuPipelineState {
	protected:
		explicit GpuShaderState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ShaderType type,
			const std::vector<Byte>& code,
			const std::string& name = "main") :
			GpuPipelineState(device),
			mCode(code), mType(type) {}

		~GpuShaderState() = default;
	public:
		auto code() const noexcept -> std::vector<Byte> { return mCode; }

		auto name() const noexcept -> std::string { return mName; }

		auto type() const noexcept -> ShaderType { return mType; }
	protected:
		std::vector<Byte> mCode = {};
		std::string mName = "";

		ShaderType mType = ShaderType::Vertex;
	};
	
}