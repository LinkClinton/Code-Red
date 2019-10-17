#include "ShaderCompiler.hpp"

#pragma comment(lib, "d3dcompiler.lib")

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include <wrl/client.h>

using namespace Microsoft;

auto ShaderCompiler::readShader(const std::string& fileName)
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

void ShaderCompiler::writeToBinary(
	const std::string& fileName,
	const std::vector<Byte>& code)
{
	std::ofstream file(fileName, std::ios::binary);

	file.write(reinterpret_cast<const char*>(code.data()), code.size());
	file.close();
}

void ShaderCompiler::writeToCpp(
	const std::string& fileName,
	const std::string& arrayName,
	const std::vector<Byte>& code)
{
	std::ofstream file(fileName);

	auto result = "constexpr unsigned char " + arrayName + "[] = {";

	for (size_t index = 0; index < code.size(); index++) {
		result = result + std::to_string(code[index]);

		if (index != code.size() - 1) result = result + ", ";
	}

	result = result + "};";

	file << result << std::endl;
	file.close();
}

auto ShaderCompiler::compileToSpv(
	const ShaderType& shaderType,
	const std::string& shader) -> std::vector<Byte>
{
	auto type = shaderc_vertex_shader;

	switch (shaderType)
	{
	case ShaderType::Vertex:
		type = shaderc_vertex_shader; break;
	case ShaderType::Pixel:
		type = shaderc_fragment_shader; break;
	default:
		return std::vector<Byte>();
	}

	const shaderc::Compiler compiler;

	const auto res = compiler.CompileGlslToSpv(shader, type, "main");

	if (res.GetNumErrors() != 0) {
		std::cout << "Compile Failed." << std::endl;
		std::cout << "Message: " << res.GetErrorMessage() << std::endl;

		return std::vector<Byte>();
	}

	auto code = std::vector<Byte>((res.end() - res.begin()) * sizeof(uint32_t));

	std::memcpy(code.data(), res.begin(), code.size());

	return code;
}

auto ShaderCompiler::compileToCso(
	const ShaderType& type,
	const std::string& shader, 
	const std::string& entry)
	-> std::vector<Byte>
{
	WRL::ComPtr<ID3DBlob> data;
	WRL::ComPtr<ID3DBlob> error;

#ifdef _DEBUG
	const UINT flag = 0;
#else
	const UINT flag = 0;
#endif

	const char* version = nullptr;

	switch (type) {
	case ShaderType::Pixel:  version = "ps_5_1"; break;
	case ShaderType::Vertex: version = "vs_5_1"; break;
	default:
		return std::vector<Byte>();
	}

	const auto res = D3DCompile(shader.data(), shader.length(),
		nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(),
		version, flag, 0, data.GetAddressOf(), error.GetAddressOf());

	if (res != S_OK && error->GetBufferSize() != 0) {
		std::cout << "Compile Failed." << std::endl;
		std::cout << "Message: ";
		
		for (size_t index = 0; index < error->GetBufferSize(); index++)
			std::cout << static_cast<char*>(error->GetBufferPointer())[index];
		std::cout << std::endl;

		return std::vector<Byte>();
	}

	auto code = std::vector<Byte>(data->GetBufferSize());

	std::memcpy(code.data(), data->GetBufferPointer(), data->GetBufferSize());

	return code;
}
