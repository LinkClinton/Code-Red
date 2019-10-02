#include "ShaderCompiler.hpp"

#include <fstream>
#include <string>

#ifdef __ENABLE__VULKAN__

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
