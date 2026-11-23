#include "utils/commandHistory.hpp"

using namespace editor;

CommandHistory::CommandHistory() {}

void CommandHistory::exec(std::unique_ptr<Command> command) {
	command->execute();
	vec.push_back(std::move(command));
}

void CommandHistory::undo() {
	if (!vec.empty()) {
		auto &last = vec.back();
		last->undo();
		redoVec.push_back(std::move(last));
		vec.pop_back();
	}
}

void CommandHistory::redo() {
	if (!redoVec.empty()) {
		auto &last = redoVec.back();
		last->execute();
		vec.push_back(std::move(last));
		redoVec.pop_back();
	}
}
