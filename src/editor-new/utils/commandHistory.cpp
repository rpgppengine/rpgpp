#include "utils/commandHistory.hpp"

using namespace editor;

CommandHistory::CommandHistory() {}

void CommandHistory::exec(std::unique_ptr<Command> command) {
	command->execute();
	vec.push_back(std::move(command));
}

void CommandHistory::undo() {
	if (!vec.empty()) {
		vec.back()->undo();
		vec.pop_back();
	}
}
