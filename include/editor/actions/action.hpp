#ifndef _RPGPP_ACTION_H
#define _RPGPP_ACTION_H

#include <functional>
class Action {
public:
	bool executeOnAdd = true;
	Action() = default;
	virtual void execute() {
		onAction();
	};
	virtual void undo() {
		onUndo();
	};

	std::function<void()> onAction;
	std::function<void()> onUndo;
};

#endif
