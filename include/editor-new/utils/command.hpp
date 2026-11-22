#ifndef _EDITOR_UTILS_COMMAND_H
#define _EDITOR_UTILS_COMMAND_H

namespace editor {
struct Command {
	virtual void execute() {};
	virtual void undo() {};
};
}  // namespace editor

#endif
