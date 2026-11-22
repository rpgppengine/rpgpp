#ifndef _EDITOR_COMMANDS_PROPSET_H
#define _EDITOR_COMMANDS_PROPSET_H

#include <functional>

#include "utils/command.hpp"
namespace editor {

template <typename T>
struct PropSet : public Command {
	T newValue;
	T old;

	std::function<void(T)> setter;
	std::function<T()> getter;

	PropSet() {};
	void execute() override {
		old = getter();
		setter(newValue);
	};
	void undo() override { setter(old); }
};
}  // namespace editor

#endif
