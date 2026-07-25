#ifndef _EDUI_GUI_H
#define _EDUI_GUI_H

#include <memory>
#include <vector>

#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct Gui {
	Color background = RAYWHITE;
	Font font;

	std::vector<std::shared_ptr<Widget>> widgets;

	std::shared_ptr<Widget> *current = nullptr;
	std::shared_ptr<Widget> *leftClickedWidget = nullptr;

	Rectangle screenRect;

	/** Last KeyboardKey. */
	KeyboardKey lastKey = KEY_NULL;

	void update();
	void draw();

	void add(std::shared_ptr<Widget> widget);

	void notifyChild(std::shared_ptr<Widget> *widget);

	Rectangle getScreenRect();
	void setFont(const char *fileName, int fontSize, const int *codepoints, int codepointCount);

	void unload();
};
}  // namespace edui

#endif
