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
			-> std::vector<Byte>;
#endif

#ifdef __ENABLE__DIRECTX12__
		static auto compileToCso(
			const ShaderType& type, 
			const std::string& shader,
			const std::string& entry = "main")
			-> std::vector<Byte>;
#endif
	};
	
}
