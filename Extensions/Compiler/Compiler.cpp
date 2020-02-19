#include "Compiler.hpp"

#include <CodeRed/Shared/Utility.hpp>

#include <shaderc/shaderc.hpp>

#include <wrl/client.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

using Microsoft::WRL::ComPtr;

auto to_shader_kind(const CodeRed::ShaderType& type) noexcept -> shaderc_shader_kind
{
	return type == CodeRed::ShaderType::Vertex ? shaderc_vertex_shader : shaderc_fragment_shader;
}

auto to_shader_model(const CodeRed::ShaderType& type) noexcept -> const wchar_t* 
{
	return type == CodeRed::ShaderType::Vertex ? L"vs_6_0" : L"ps_6_0";
}

template<typename Result>
auto convert_blob_to_result(const ComPtr<IDxcBlob>& blob) -> Result;

template<>
auto convert_blob_to_result(const ComPtr<IDxcBlob>& blob) -> std::string
{
	std::string result;

	result.resize(blob->GetBufferSize());

	std::memcpy(result.data(), blob->GetBufferPointer(), blob->GetBufferSize());

	if (!result.empty() && result.back() == '\n') result.pop_back();
	
	return result;
}

template<>
auto convert_blob_to_result(const ComPtr<IDxcBlob>& blob) -> CodeRed::Compiler::ByteCode
{
	CodeRed::Compiler::ByteCode result;

	result.resize(blob->GetBufferSize());

	std::memcpy(result.data(), blob->GetBufferPointer(), blob->GetBufferSize());

	return result;
}

auto CodeRed::Compiler::compileHLSLToDXIL(const std::string& code, const CompileOption& option) -> CompileResult
{
	ComPtr<IDxcOperationResult> result;
	ComPtr<IDxcBlobEncoding> source;
	ComPtr<IDxcCompiler> compiler;
	ComPtr<IDxcLibrary> library;
	HRESULT status;

	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

	library->CreateBlobWithEncodingFromPinned(code.data(), static_cast<UINT32>(code.length()), CP_UTF8, &source);
	compiler->Compile(source.Get(), nullptr, L"main", to_shader_model(option.Type),
		nullptr, 0, nullptr, 0, nullptr, &result);

	result->GetStatus(&status);
	
	ComPtr<IDxcBlobEncoding> message;
	ComPtr<IDxcBlob> dxilCode;
	
	result->GetErrorBuffer(&message);
	result->GetResult(&dxilCode);

	return {
		convert_blob_to_result<std::string>(message),
		convert_blob_to_result<ByteCode>(dxilCode)
	};
}

auto CodeRed::Compiler::compileHLSLToSPIRV(const std::string& code, const CompileOption& option) -> CompileResult
{
	shaderc::CompileOptions options;
	shaderc::Compiler compiler;

	options.SetSourceLanguage(shaderc_source_language_hlsl);

	const auto result = compiler.CompileGlslToSpv(code.c_str(), code.length(),
		to_shader_kind(option.Type), "main", options);

	auto spirvCode = std::vector<Byte>((result.end() - result.begin()) * sizeof(uint32_t));

	std::memcpy(spirvCode.data(), result.begin(), spirvCode.size());

	return { result.GetErrorMessage(), spirvCode };
}

auto CodeRed::Compiler::compileGLSLToDXIL(const std::string& code, const CompileOption& option) -> CompileResult
{

	throw std::exception("Not Implementation.");
}

auto CodeRed::Compiler::compileGLSLToSPIRV(const std::string& code, const CompileOption& option) -> CompileResult
{
	shaderc::CompileOptions options;
	shaderc::Compiler compiler;

	options.SetSourceLanguage(shaderc_source_language_glsl);
	
	const auto result = compiler.CompileGlslToSpv(code.c_str(), code.length(),
		to_shader_kind(option.Type), "main", options);

	auto spirvCode = std::vector<Byte>((result.end() - result.begin()) * sizeof(uint32_t));

	std::memcpy(spirvCode.data(), result.begin(), spirvCode.size());

	return { result.GetErrorMessage(), spirvCode };
}

auto CodeRed::Compiler::compile(const std::string& code, const CompileOption& option) -> CompileResult
{
	if (option.Source == SourceLanguage::eHLSL && option.Target == TargetLanguage::eDXIL)
		return compileHLSLToDXIL(code, option);

	if (option.Source == SourceLanguage::eHLSL && option.Target == TargetLanguage::eSPIRV)
		return compileHLSLToSPIRV(code, option);

	if (option.Source == SourceLanguage::eGLSL && option.Target == TargetLanguage::eDXIL)
		return compileGLSLToDXIL(code, option);

	if (option.Source == SourceLanguage::eGLSL && option.Target == TargetLanguage::eSPIRV)
		return compileGLSLToSPIRV(code, option);

	throw std::exception("Not Implementation.");
}


