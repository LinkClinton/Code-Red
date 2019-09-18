#pragma once

#include <Interface/GpuPipelineState/GpuShaderState.hpp>
#include <DirectX12/DirectX12Utility.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12ShaderState final : public GpuShaderState {
	public:
		DirectX12ShaderState() = default;

		explicit DirectX12ShaderState(
			const std::vector<Byte>& code);

		auto constexpr shader() const noexcept -> D3D12_SHADER_BYTECODE { return mShaderCode; }
	private:
		D3D12_SHADER_BYTECODE mShaderCode = { nullptr, 0 };
	};
	
}

#endif