#pragma once

#include <Core/CodeRedGraphics.hpp>

#ifdef __ENABLE__VULKAN__
#include <shaderc/shaderc.hpp>
#endif

#include <string>

namespace CodeRed {

	class ShaderCompiler {
	public:
#ifdef __ENABLE__VULKAN__
		
		static auto compileToSpv(const ShaderType& shaderType, const std::string& shader)
			->	shaderc::SpvCompilationResult;
#endif
	};
	
}
