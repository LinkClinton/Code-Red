#include "DirectX12ShaderState.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12ShaderState::DirectX12ShaderState(
	const std::vector<Byte>& code)
	: GpuShaderState(code)
{
	mShaderCode.BytecodeLength = mCode.size();
	mShaderCode.pShaderBytecode = mCode.data();
}

#endif