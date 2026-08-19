#ifndef _EDUI_GUI_H
#define _EDUI_GUI_H

#include <memory>
#include <vector>

#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
const float EDUI_DEFAULT_HEIGHT = 16;

struct Gui {
	static Gui* instance;

	Color background = RAYWHITE;
	Font font;
	float labelFontSize = 16;
	float fontSpacing = 0;

	bool hasMenuBar = false;
	std::shared_ptr<Widget> menuBar = nullptr;

	Vector2 lastMousePos;

	std::vector<std::shared_ptr<Widget>> widgets;
	std::vector<std::shared_ptr<Widget>> topLayer;

	std::shared_ptr<Widget> *current = nullptr;
	std::weak_ptr<Widget> leftClickedWidget;
	std::weak_ptr<Widget> middleClickedWidget;

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
	void processWidget(std::shared_ptr<Widget>& widget);

	void add(std::shared_ptr<Widget> widget);
	void addTop(std::shared_ptr<Widget> widget);

	void notifyChild(std::shared_ptr<Widget> *widget);

	Rectangle getScreenRect();
	void setFont(const char *fileName, int fontSize, int labelFontSize, int fontSpacing);

	void addMenuBar(std::shared_ptr<Widget> widget);

	void unload();
};
}  // namespace edui

#endif
