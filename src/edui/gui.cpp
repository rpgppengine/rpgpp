#include "edui/gui.hpp"
#include <memory>
#include <string_view>

#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

Gui* Gui::instance = nullptr;

void Gui::processWidget(std::shared_ptr<Widget>& widget) {
	if (widget->mouseIsInRect()) {
		if (!widget->isContainer) {
			notifyChild(&widget);
		} else {
			notifyChild(&widget);
			widget->as<Container>().notifyChildren(this);
		}
	} else {
		if (widget->notifiedMouseEnter) {
			widget->notifiedMouseEnter = false;
			widget->mouseLeft();
		}

		if (widget->deleteOnOutsideClick) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				widget->markDelete();
			}
		}
	}

	widget->update();
	widget->calcRect(screenRect);

	if (auto leftPtr = leftClickedWidget.lock()) {
		if (leftPtr->deleteFlag) {
			leftClickedWidget.reset();
		}
	}
}

void Gui::processVector(std::vector<std::shared_ptr<Widget>>& vec) {
	int i = 0;
	for (auto &widget : vec) {
		if (widget->deferFlag) {
			widget->deferFlag = false;
			continue;
		}

		processWidget(widget);

		if (widget->deleteFlag) {
			widget->onDeleted.invoke();
			if (widget->isContainer) {
				widget->as<Container>().deleteChildren();
			}
			vec.erase(vec.begin() + i);
		}

		i++;
	}
}

void Gui::update() {
	screenRect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
	if (hasMenuBar && menuBar != nullptr) {
		if (menuBar->deferFlag) {
			menuBar->deferFlag = false;
			return;
		}

		processWidget(menuBar);

		float menuBarHeight = menuBar->rect.height;
		screenRect.y += menuBarHeight;
		screenRect.height -= menuBarHeight;
	}

	processVector(topLayer);
	processVector(widgets);

	if (current != nullptr) {
		KeyboardKey key = static_cast<KeyboardKey>(GetKeyPressed());
		KeyModifier mod;
		mod.alt = IsKeyDown(KEY_LEFT_ALT);
		mod.ctrl = IsKeyDown(KEY_LEFT_CONTROL);
		mod.shift = IsKeyDown(KEY_LEFT_SHIFT);
		int codepoint = GetCharPressed();

		if (key == KEY_KP_7 && codepoint == 0) {
			mod.numlock = true;
		}
		if (key == KEY_KP_1 && codepoint == 0) {
			mod.numlock = true;
		}

		if (key != KEY_NULL) {
			lastKey = key;
			current->get()->keyPressed(key, mod, false);
		}

		if (lastKey != KEY_NULL && IsKeyDown(lastKey)) {
			current->get()->keyPressed(lastKey, mod, true);
		}

		while (codepoint != 0) {
			int size = 0;
			auto codepointStrC = CodepointToUTF8(codepoint, &size);
			std::string_view view = codepointStrC;
			current->get()->charEntered(codepoint, view);

			codepoint = GetCharPressed();
		}
	}

	if (auto leftPtr = leftClickedWidget.lock()) {
		if (!leftPtr->deleteFlag) {
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				leftPtr->leftMouseReleased();
			}
		}
	}

	if (auto middlePtr = leftClickedWidget.lock()) {
		if (!middlePtr->deleteFlag) {
			if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) {
				middlePtr->leftMouseReleased();
			}
		}
	}
}

void Gui::draw() {
	ClearBackground(background);

	if (hasMenuBar && menuBar != nullptr) {
		menuBar->draw();
	}

	for (auto &widget : widgets) {
		if (widget->visible) {
			widget->draw();
		}
	}
	for (auto &widget : topLayer) {
		if (widget->visible) {
			widget->draw();
		}
	}
}

void Gui::add(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this;
		for (auto& subwidget : widget->as<Container>().widgets) {
			subwidget->render->font = &this->font;
		}
	}
	widget->render->font = &this->font;
	widget->unfocused();
	widgets.push_back(widget);
}

void Gui::addTop(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this;
		for (auto& subwidget : widget->as<Container>().widgets) {
			subwidget->render->font = &this->font;
		}
	}
	widget->render->font = &this->font;
	widget->unfocused();
	topLayer.push_back(widget);
}

void Gui::notifyChild(std::shared_ptr<Widget> *widget) {
	widget->get()->scrolled(GetMouseWheelMove());

	if (!widget->get()->notifiedMouseEnter) {
		widget->get()->mouseEntered();
		widget->get()->notifiedMouseEnter = true;
	}

	if (!FloatEquals(GetMouseDelta().x, 0.0f)) {
		auto mousePos = GetMousePosition();
		auto relative = mousePos;
		relative.x -= widget->get()->rect.x;
		relative.y -= widget->get()->rect.y;
		widget->get()->mouseMoved(GetMousePosition(), relative);
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		leftClickedWidget = *widget;
		widget->get()->leftMouseClicked();

		if (widget->get()->focusable) {
			if (current != nullptr) {
				current->get()->unfocused();
			}
			current = widget;
			current->get()->focused();
		}
	}
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		widget->get()->rightMouseClicked();
	}

	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
		middleClickedWidget = *widget;
		widget->get()->middleMouseClicked();
	}
}

void Gui::addMenuBar(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this;
		for (auto& subwidget : widget->as<Container>().widgets) {
			subwidget->render->font = &this->font;
		}
	}
	widget->render->font = &this->font;
	widget->unfocused();

	menuBar = widget;
	menuBar->setSize({1, 0}, {0, 22});
	hasMenuBar = true;
}

Rectangle Gui::getScreenRect() {
	return {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
}

void Gui::setFont(const char *fileName, int fontSize, int labelFontSize, int fontSpacing) {
	if (IsFontValid(font)) {
		UnloadFont(font);
	}

	this->labelFontSize = labelFontSize;
	this->fontSpacing = fontSpacing;

	auto codepoints = loadFontCodepoints();
	font = LoadFontEx(fileName, fontSize, codepoints.data(), codepoints.size());
}

void Gui::unload() {
	if (IsFontValid(font)) {
		UnloadFont(font);
	}
}
