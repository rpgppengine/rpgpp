#ifndef _EDITOR_UTILS_COMMANDHISTORY_H
#define _EDITOR_UTILS_COMMANDHISTORY_H

#include <memory>
#include <vector>

#include "utils/command.hpp"
namespace editor {
class CommandHistory {
public:
	CommandHistory();
	void exec(std::unique_ptr<Command> command);
	void undo();
	void redo();

private:
	std::vector<std::unique_ptr<Command>> vec;
	std::vector<std::unique_ptr<Command>> redoVec;
};
}  // namespace editor

#endif
