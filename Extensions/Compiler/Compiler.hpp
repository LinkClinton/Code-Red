#pragma once

#include "ByteCode.hpp"
#include "CompileOption.hpp"

#include <string>

namespace CodeRed {

	namespace Compiler {

		struct CompileResult {
			std::string Message;
			
			ByteCode Code;

			CompileResult() = default;

			auto success() const noexcept -> bool { return !failed(); }

			auto failed() const noexcept -> bool { return Code.empty(); }
		};

		auto compileHLSLToDXIL(const std::string& code, const CompileOption& option) -> CompileResult;

		auto compileHLSLToSPIRV(const std::string& code, const CompileOption& option) -> CompileResult;

		auto compileGLSLToDXIL(const std::string& code, const CompileOption& option) -> CompileResult;

		auto compileGLSLToSPIRV(const std::string& code, const CompileOption& option) -> CompileResult;

		auto compile(const std::string& code, const CompileOption& option) -> CompileResult;
	}
	
}
