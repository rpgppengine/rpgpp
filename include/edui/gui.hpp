#ifndef _EDUI_GUI_H
#define _EDUI_GUI_H

#include <array>
#include <cstddef>
#include <memory>
#include <vector>

#include "edui/widget.hpp"
#include "ini.h"
#include "raylib.h"

namespace edui {
const int EDUI_MAX_LAYERS = 5;
const float EDUI_DEFAULT_HEIGHT = 16;
const float EDUI_SECONDARY_HEIGHT = 26;

struct LanguageName {
	Str128 key;
	Str128 value;
};
const size_t EDUI_MAX_LANGUAGES = 128;

struct Gui {
	static Gui *instance;

	Color background = RAYWHITE;
	Font font;
	float labelFontSize = 16;
	float fontSpacing = 0;

	bool hasMenuBar = false;
	std::shared_ptr<Widget> menuBar = nullptr;

	Vector2 lastMousePos;

	std::array<std::vector<std::shared_ptr<Widget>>, EDUI_MAX_LAYERS> arr = {};

	bool notified = false;
	std::shared_ptr<Widget> *current = nullptr;
	std::weak_ptr<Widget> leftClickedWidget;
	std::weak_ptr<Widget> middleClickedWidget;

	Rectangle screenRect;

	/** Last KeyboardKey. */
	KeyboardKey lastKey = KEY_NULL;

	mINI::INIStructure translationStruct;
	std::array<LanguageName, EDUI_MAX_LANGUAGES> languageNames = {};
	size_t languageNamesCount = 0;

	Gui() {
		Gui::instance = this;
		font = GetFontDefault();
	}

	void update();
	void draw();

	void processVector(std::vector<std::shared_ptr<Widget>> &vec);
	void processWidget(std::shared_ptr<Widget> &widget);

	void add(std::shared_ptr<Widget> widget, int layerId = 0);

	void notifyChild(std::shared_ptr<Widget> *widget);

	Rectangle getScreenRect();
	void setFont(const char *fileName, int fontSize, int labelFontSize, int fontSpacing);

	void addMenuBar(std::shared_ptr<Widget> widget);

	void loadTranslationNames(const std::string &filePath);
	void loadTranslation(const std::string &filePath);

	void unload();
};
}  // namespace edui

#endif
