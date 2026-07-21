#ifndef _EDUI_GUI_H
#define _EDUI_GUI_H

#include <memory>
#include <vector>

#include "edui/widget.hpp"
namespace edui {
struct Gui {
	std::vector<std::shared_ptr<Widget>> widgets;

	std::shared_ptr<Widget>* current = nullptr;

	/** Last KeyboardKey. */
	KeyboardKey lastKey = KEY_NULL;

	void update();
	void draw();

	void add(std::shared_ptr<Widget> widget);

	void notifyChild(std::shared_ptr<Widget>* widget);
};
}  // namespace edui

#endif
