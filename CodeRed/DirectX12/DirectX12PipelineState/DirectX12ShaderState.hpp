#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuShaderState.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../../Interface/GpuPipelineState/GpuShaderState.hpp"
#include "../DirectX12Utility.hpp"
#endif

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

		auto constexpr shader() const noexcept -> D3D12_SHADER_BYTECODE { return mShaderCode; }
	private:
		D3D12_SHADER_BYTECODE mShaderCode = { nullptr, 0 };
	};
	
}

#endif