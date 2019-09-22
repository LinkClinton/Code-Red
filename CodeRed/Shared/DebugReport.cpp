#include "Exception/NotSupportException.hpp"
#include "Exception/AssertException.hpp"
#include "DebugReport.hpp"

#include <iostream>

void CodeRed::DebugReport::warning(
	const DebugType& type, 
	const std::vector<std::string>& messages)
{
	std::cout << "warning : " << push(select(type), messages) << std::endl;
}

void CodeRed::DebugReport::warning(const std::string& message)
{
	std::cout << "warning : " << message << std::endl;
}

auto CodeRed::DebugReport::select(const Type type) -> std::string
{
	switch (type) {
	case Type::Create: return "Create [0] failed.";
	case Type::Get: return "Get [0] failed from [1].";
	case Type::Set: return "Set [0] failed because [1].";
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DebugReport::push(const std::string& message_template,
	const std::vector<std::string>& messages) -> std::string
{
	assert(messages.size() <= MaxMessages);

	std::string message;

	for (size_t index = 0; index < message_template.size(); index++) {
		message.push_back(message_template[index]);

		if (index == 0 || index == message_template.size() - 1) continue;

		//Because the MaxMessages is 10, the x is [0, 9]
		//We can check the message_template[index - 1, index + 1] is equal [x]
		if (message_template[index] >= '0' && message_template[index] <= '9' &&
			message_template[index - 1] == '[' && message_template[index + 1] == ']') {
			const auto location = message_template[index] - '0';

			assert(location < messages.size());

			//Because we add "[x" to message, we need to pop them.
			//Then we replace messages[x] to [x] in message_template
			message.pop_back();
			message.pop_back();
			message.append(messages[location]);

			//skip the "]" char.
			index++;
		}
	}

	return message;
}
