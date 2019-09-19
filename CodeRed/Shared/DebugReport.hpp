#pragma once

#include <Shared/Utility.hpp>

#include <vector>
#include <string>

namespace CodeRed {

	enum class DebugType
	{
		//Create [0] failed.
		Create,
		//Get [0] failed from [1].
		Get,
		//Set [0] failed because [1].
		Set
	};
	
	class DebugReport final {
	public:
		using Type = DebugType;
		const static size_t MaxMessages = 10;
	public:
		static void warning(
			const DebugType& type,
			const std::vector<std::string>& messages);
	private:
		static auto select(const Type type) -> std::string;

		static auto push(
			const std::string& message_template, 
			const std::vector<std::string>& messages) -> std::string;

		template<typename T>
		friend class InvalidException;
		friend class FailedException;
	};
	
}
