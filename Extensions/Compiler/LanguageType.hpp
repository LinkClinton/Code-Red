#pragma once

namespace CodeRed {

	namespace Compiler {

		enum class SourceLanguage : unsigned {
			eHLSL = 0,
			eGLSL = 1
		};

		enum class TargetLanguage : unsigned {
			eDXIL = 0,
			eSPIRV = 1
		};
		
	}
	
}
