#pragma once

#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

using CommandFunction = std::function<bool(void*, const std::string&)>;

class CommandList {
public:
	void setCommand(const std::string& command, const CommandFunction& function);

	bool execute(void* context, const std::vector<std::string>& commands);

	static auto read_from_argv(int argc, char** argv) -> std::vector<std::string> ;
private:
	std::map<std::string, CommandFunction> mCommands;
};

inline void CommandList::setCommand(const std::string& command, const CommandFunction& function) {
	mCommands.insert(std::make_pair(command, function));
}

inline bool CommandList::execute(void* context, const std::vector<std::string>& commands) {
	std::vector<std::string> local_commands;
	std::queue<std::string> local_params;

	for (const auto& command : commands) {
		if (command[0] == '-') local_commands.push_back(command);
		else local_params.push(command);
	}

	std::set<std::string> visit;

	for (const auto& command : local_commands) {
		if (visit.find(command) != visit.end()) {
			std::cout << "error : repeat command." << std::endl;
			
			return false;
		}

		visit.insert(command);

		if (mCommands.find(command) == mCommands.end()) {
			std::cout << "error : invalid command" << std::endl;
			return false;
		}

		if (mCommands[command](context, local_params.front()) == false) return false;

		local_params.pop();
	}

	return true;
}

inline auto CommandList::read_from_argv(int argc, char** argv) -> std::vector<std::string> {
	std::vector<std::string> result;

	for (auto i = 1; i < argc; i++) result.push_back(argv[i]);

	return result;
}
	