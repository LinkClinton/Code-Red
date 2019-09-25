#include "DirectX12ShaderState.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12ShaderState::DirectX12ShaderState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const ShaderType type,
	const std::vector<Byte>& code,
	const std::string& name)
	: GpuShaderState(device, type, code, name)
{
	mShaderCode.BytecodeLength = mCode.size();
	mShaderCode.pShaderBytecode = mCode.data();
}

#endif