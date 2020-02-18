#include "Compiler.hpp"

#include <shaderc/shaderc.hpp>

auto to_shader_kind(const CodeRed::ShaderType& type) noexcept -> shaderc_shader_kind
{
	return type == CodeRed::ShaderType::Vertex ? shaderc_vertex_shader : shaderc_fragment_shader;
}

auto CodeRed::Compiler::compileHLSLToDXIL(const std::string& code, const CompileOption& option) -> CompileResult
{
	throw std::exception("Not Implementation.");
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


