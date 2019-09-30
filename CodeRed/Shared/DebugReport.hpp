#pragma once

#include "Enum/DebugType.hpp"

#include <vector>
#include <string>

namespace CodeRed {
	
	class DebugReport final {
	public:
		using Type = DebugType;
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
	private:
		static void output(const std::string& text);

		static auto select(const Type type) -> std::string;

		static auto merge(const std::vector<std::string>& messages)
			-> std::string;
		
		static auto push(
			const std::string& format, 
			const std::vector<std::string>& messages) -> std::string;

		friend class Exception;
	};
	
}
