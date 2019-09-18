#include "DirectX12ShaderState.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12ShaderState::DirectX12ShaderState(
	const std::vector<Byte>& code)
	: GpuShaderState(code)
{
	mShaderCode.BytecodeLength = code.size();
	mShaderCode.pShaderBytecode = code.data();
}

#endif