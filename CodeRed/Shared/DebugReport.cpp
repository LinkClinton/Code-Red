#include "Exception/NotSupportException.hpp"
#include "DebugReport.hpp"

#include <iostream>
#include <cassert>


void CodeRed::DebugReport::warning(const std::string& message)
{
	return output("cr-warning : " + message);
}

void CodeRed::DebugReport::warning(
	const std::string& format,
	const std::vector<std::string>& messages)
{
	return warning(push(format, messages));
}

void CodeRed::DebugReport::warning(
	const DebugType& type,
	const std::vector<std::string>& messages,
	const std::vector<std::string>& debug_message)
{
	return warning(push(select(type), messages) + 
		"\ncr-debug message : " + merge(debug_message));
}

void CodeRed::DebugReport::warning(
	const DebugType& type,
	const std::vector<std::string>& messages)
{
	return warning(push(select(type), messages));
}

void CodeRed::DebugReport::error(const std::string& message)
{
	return output("cr-error : " + message);
}

void CodeRed::DebugReport::error(
	const std::string& format, 
	const std::vector<std::string>& messages)
{
	return error(push(format, messages));
}

void CodeRed::DebugReport::error(
	const DebugType& type,
	const std::vector<std::string>& messages,
	const std::vector<std::string>& debug_message)
{
	return error(push(select(type), messages) +
		"\ncr-debug message : " + merge(debug_message));
}

void CodeRed::DebugReport::error(
	const DebugType& type,
	const std::vector<std::string>& messages)
{
	return error(push(select(type), messages));
}

void CodeRed::DebugReport::message(
	const std::string& message)
{
	return output("cr-message : " + message);
}

void CodeRed::DebugReport::message(
	const std::string& format, 
	const std::vector<std::string>& messages)
{
	return message(push(format, messages));
}

auto CodeRed::DebugReport::makeError(
	const std::string& message)
	-> std::string
{
	return "cr-error : " + message;
}

auto CodeRed::DebugReport::makeError(
	const std::string& format,
	const std::vector<std::string>& messages)
	-> std::string
{
	return makeError(push(format, messages));
}

auto CodeRed::DebugReport::makeError(
	const std::string& format, 
	const std::vector<std::string>& messages,
	const std::vector<std::string>& debug_message)
	-> std::string
{
	return makeError(push(format, messages) +
		"\ncr-debug message : " + merge(debug_message));
}

auto CodeRed::DebugReport::makeError(
	const DebugType& type, 
	const std::vector<std::string>& messages,
	const std::vector<std::string>& debug_message)
	-> std::string
{
	return makeError(select(type), messages, debug_message);
}

auto CodeRed::DebugReport::makeError(
	const DebugType& type,
	const std::vector<std::string>& messages)
	-> std::string
{
	return makeError(select(type), messages);
}

auto CodeRed::DebugReport::make(
	const std::string& format, 
	const std::vector<std::string>& messages)
	-> std::string
{
	return push(format, messages);
}

auto CodeRed::DebugReport::make(
	const DebugType& type, 
	const std::vector<std::string> messages,
	const std::vector<std::string> debug_message)
	-> std::string
{
	return push(select(type), messages) +
		"\ncr-debug message : " + merge(debug_message);
}

void CodeRed::DebugReport::output(const std::string& text)
{
	std::cout << text << std::endl;
}

auto CodeRed::DebugReport::select(const DebugType type) -> std::string
{
	switch (type) {
	case DebugType::Create: return "create [0] failed.";
	case DebugType::Get: return "get [0] from [1] failed.";
	case DebugType::Set: return "set [0] to [1] failed.";
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DebugReport::merge(const std::vector<std::string>& messages)
	-> std::string
{
	if (messages.empty()) return "sorry, no messages.";

	std::string res = messages[0];

	for (size_t index = 1; index < messages.size(); index++)
		res = res + ", " + messages[index];

	return res;
}

auto CodeRed::DebugReport::push(const std::string& format,
	const std::vector<std::string>& messages) -> std::string
{
	assert(messages.size() <= MaxMessages);

	std::string message;

	for (size_t index = 0; index < format.size(); index++) {
		message.push_back(format[index]);

		if (index == 0 || index == format.size() - 1) continue;

		//Because the MaxMessages is 10, the x is [0, 9]
		//We can check the format[index - 1, index + 1] is equal [x]
		if (format[index] >= '0' && format[index] <= '9' &&
			format[index - 1] == '[' && format[index + 1] == ']') {
			const auto location = format[index] - '0';

			//Because we add "[x" to message, we need to pop them.
			//Then we replace messages[x] to [x] in format
			message.pop_back();
			message.pop_back();

			//if there no messages need to push
			//we ignore the [x]
			if (location < messages.size())
				message.append(messages[location]);

			//skip the "]" char.
			index++;
		}
	}

	return message;
}
