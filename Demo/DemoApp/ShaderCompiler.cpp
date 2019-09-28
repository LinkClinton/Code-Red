#include "ShaderCompiler.hpp"

#ifdef __ENABLE__VULKAN__

auto CodeRed::ShaderCompiler::compileToSpv(
	const ShaderType& shaderType, 
	const std::string& shader)
	-> shaderc::SpvCompilationResult
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

	auto result = compiler.CompileGlslToSpv(shader, type, "main");

	if (result.GetNumErrors() != 0 || result.GetNumWarnings() != 0)
		DebugReport::error(result.GetErrorMessage());
	
	return result;
}
#endif
