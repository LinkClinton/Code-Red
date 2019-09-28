#pragma once

#include "../../Interface/GpuPipelineState/GpuShaderState.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12ShaderState final : public GpuShaderState {
	public:
		explicit DirectX12ShaderState(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const ShaderType type,
			const std::vector<Byte>& code,
			const std::string &name = "main");

		~DirectX12ShaderState() = default;

		auto shader() const noexcept -> D3D12_SHADER_BYTECODE { return mShaderCode; }
	private:
		D3D12_SHADER_BYTECODE mShaderCode = { nullptr, 0 };
	};
	
}

#endif