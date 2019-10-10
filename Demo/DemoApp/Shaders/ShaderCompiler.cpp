#include "ShaderCompiler.hpp"

#ifdef __ENABLE__DIRECTX12__
#include <d3dcompiler.h>
#endif

#ifdef __ENABLE__VULKAN__
#include <shaderc/shaderc.hpp>
#endif

#include <fstream>
#include <string>

auto CodeRed::ShaderCompiler::readShader(const std::string& fileName)
	-> std::string
{
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);

	assert(file.is_open());

	const auto size = static_cast<size_t>(file.tellg());

	std::string code; code.resize(size);

	file.seekg(0, std::ios::beg);
	file.read(static_cast<char*>(code.data()), size);
	file.close();

	return code;
}

#ifdef __ENABLE__VULKAN__

auto CodeRed::ShaderCompiler::compileToSpv(
	const ShaderType& shaderType, 
	const std::string& shader)
	-> std::vector<Byte>
{
	auto type = shaderc_vertex_shader;

	switch (shaderType)
	{
	case ShaderType::Vertex:
		type = shaderc_vertex_shader; break;
	case ShaderType::Pixel:
		type = shaderc_fragment_shader; break;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}

	const shaderc::Compiler compiler;

	const auto res = compiler.CompileGlslToSpv(shader, type, "main");

	if (res.GetNumErrors() != 0) {
		DebugReport::error(res.GetErrorMessage());

		throw FailedException(DebugType::Create, { "compile shader failed." });
	}

	auto code = std::vector<Byte>((res.end() - res.begin()) * sizeof(uint32_t));
	
	std::memcpy(code.data(), res.begin(), code.size());

	return code;
}

#endif

#ifdef __ENABLE__DIRECTX12__

auto CodeRed::ShaderCompiler::compileToCso(
	const ShaderType& type, 
	const std::string& shader,
	const std::string& entry)
	-> std::vector<Byte>
{
	WRL::ComPtr<ID3DBlob> data;
	WRL::ComPtr<ID3DBlob> error;

#ifdef _DEBUG
	const UINT flag = D3DCOMPILE_DEBUG;
#else
	const UINT flag = 0;
#endif

	const char* version = nullptr;
	
	switch (type) {
	case ShaderType::Pixel:  version = "ps_5_1"; break;
	case ShaderType::Vertex: version = "vs_5_1"; break;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}

	const auto res = D3DCompile(shader.data(), shader.length(),
		nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(),
		version, flag, 0, data.GetAddressOf(), error.GetAddressOf());

	if (res != S_OK && error->GetBufferSize() != 0) {
		DebugReport::error(
			DirectX12::charArrayToString(
				error->GetBufferPointer(), error->GetBufferSize()));

		throw FailedException(DebugType::Create, { "compile shader failed." });
	}

	auto code = std::vector<Byte>(data->GetBufferSize());

	std::memcpy(code.data(), data->GetBufferPointer(), data->GetBufferSize());

	return code;
}

#endif
