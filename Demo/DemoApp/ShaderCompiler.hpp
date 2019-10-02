#pragma once

#include <Core/CodeRedGraphics.hpp>

#ifdef __ENABLE__VULKAN__
#include <shaderc/shaderc.hpp>
#endif

#include <string>

namespace CodeRed {

	class ShaderCompiler {
	public:
		static auto readShader(const std::string& fileName) -> std::string;
		
#ifdef __ENABLE__VULKAN__

		static auto compileToSpv(const ShaderType& shaderType, const std::string& shader)
			->	shaderc::SpvCompilationResult;
#endif
	};
	
}
