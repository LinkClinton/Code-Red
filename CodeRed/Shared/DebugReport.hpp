#pragma once

#include "Enum/DebugType.hpp"

#include <vector>
#include <string>

#undef CODE_RED_DEBUG_LOG
#undef CODE_RED_DEBUG_WARNING
#undef CODE_RED_DEBUG_WARNING_IF

#ifdef __ENABLE__CODE__RED__DEBUG__
#define CODE_RED_DEBUG_LOG(log) DebugReport::message(log);
#define CODE_RED_DEBUG_WARNING(message) DebugReport::warning(message);
#define CODE_RED_DEBUG_WARNING_IF(condition, message) \
	if (condition) DebugReport::warning(message);
#else
#define CODE_RED_DEBUG_LOG(message)
#define CODE_RED_DEBUG_WARNING(message)
#define CODE_RED_DEBUG_WARNING_IF(condition, message)
#endif

namespace CodeRed {
	
	class DebugReport final {
	public:
		const static size_t MaxMessages = 10;
	public:
		static void warning(
			const std::string& message);
		
		static void warning(
			const std::string& format,
			const std::vector<std::string>& messages);
		
		static void warning(
			const DebugType& type,
			const std::vector<std::string>& messages,
			const std::vector<std::string>& debug_message);
		
		static void warning(
			const DebugType& type,
			const std::vector<std::string>& messages);
		
		static void error(
			const std::string& message);

		static void error(
			const std::string& format,
			const std::vector<std::string>& messages);

		static void error(
			const DebugType& type,
			const std::vector<std::string>& messages,
			const std::vector<std::string>& debug_message);

		static void error(
			const DebugType& type,
			const std::vector<std::string>& messages);
		
		static void message(
			const std::string& message);

		static void message(
			const std::string& format,
			const std::vector<std::string>& messages);
		
		static auto makeError(
			const std::string& message)
			->std::string;

		static auto makeError(
			const std::string& format,
			const std::vector<std::string>& messages)
			->std::string;

		static auto makeError(
			const std::string& format,
			const std::vector<std::string>& messages,
			const std::vector<std::string>& debug_message)
			-> std::string;

		static auto makeError(
			const DebugType& type,
			const std::vector<std::string>& messages,
			const std::vector<std::string>& debug_message)
			->std::string;

		static auto makeError(
			const DebugType& type,
			const std::vector<std::string>& messages)
			->std::string;

		static auto make(
			const std::string& format,
			const std::vector<std::string>& messages)
			-> std::string;

		static auto make(
			const DebugType& type,
			const std::vector<std::string> messages,
			const std::vector<std::string> debug_message)
			-> std::string;
	private:
		static void output(const std::string& text);

		static auto select(const DebugType type) -> std::string;

		static auto merge(const std::vector<std::string>& messages)
			-> std::string;
		
		static auto push(
			const std::string& format, 
			const std::vector<std::string>& messages) -> std::string;

		friend class Exception;
	};
	
}
