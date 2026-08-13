#ifndef _EDUI_GUI_H
#define _EDUI_GUI_H

#include <memory>
#include <vector>

#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct Gui {
	static Gui* instance;

	Color background = RAYWHITE;
	Font font;

	Vector2 lastMousePos;

	std::vector<std::shared_ptr<Widget>> widgets;
	std::vector<std::shared_ptr<Widget>> topLayer;

	std::shared_ptr<Widget> *current = nullptr;
	std::shared_ptr<Widget> *leftClickedWidget = nullptr;
	std::shared_ptr<Widget> *middleClickedWidget = nullptr;

	Rectangle screenRect;

	/** Last KeyboardKey. */
	KeyboardKey lastKey = KEY_NULL;

	Gui() {
		Gui::instance = this;
		font = GetFontDefault();
	}

	void update();
	void draw();

	void processVector(std::vector<std::shared_ptr<Widget>>& vec);

	void add(std::shared_ptr<Widget> widget);
	void addTop(std::shared_ptr<Widget> widget);

	void notifyChild(std::shared_ptr<Widget> *widget);

	Rectangle getScreenRect();
	void setFont(const char *fileName, int fontSize);

	void unload();
};
}  // namespace edui

#endif
