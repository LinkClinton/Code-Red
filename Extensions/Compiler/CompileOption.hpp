#pragma once

#include <CodeRed/Shared/Enum/ShaderType.hpp>

#include "LanguageType.hpp"

namespace CodeRed {

	namespace Compiler {

		struct HLSLCompileOption {
			
		};

		struct GLSLCompileOption {
			
		};
		
		struct CompileOption {
			SourceLanguage Source = SourceLanguage::eHLSL;
			TargetLanguage Target = TargetLanguage::eDXIL;

			ShaderType Type = ShaderType::Vertex;
			
			HLSLCompileOption HLSL;
			GLSLCompileOption GLSL;

			CompileOption() = default;

			CompileOption(
				const SourceLanguage& source,
				const TargetLanguage& target,
				const ShaderType& type) :
				Source(source), Target(target), Type(type) {}
		};
	}
	
}
