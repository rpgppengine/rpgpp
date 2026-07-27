#include "edui/gui.hpp"
#include <memory>

#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

Gui* Gui::instance = nullptr;

void Gui::processVector(std::vector<std::shared_ptr<Widget>>& vec) {
	int i = 0;
	for (auto &widget : vec) {
		if (widget->mouseIsInRect()) {
			if (!widget->isContainer) {
				notifyChild(&widget);
			} else {
				widget->scrolled(GetMouseWheelMove());
				notifyChild(&widget);
				widget->as<Container>().notifyChildren(this);
			}
		} else {
			if (widget->notifiedMouseEnter) {
				widget->notifiedMouseEnter = false;
				widget->mouseLeft();
			}
		}

		widget->update();
		widget->calcRect(screenRect);

		if (widget->deleteFlag) {
			vec.erase(vec.begin() + i);
		}

		i++;
	}
}

void Gui::update() {
	Rectangle screenRect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};

	processVector(topLayer);
	processVector(widgets);

	if (current != nullptr) {
		int c = GetCharPressed();
		while (c != 0) {
			current->get()->charEntered(static_cast<char>(c));
			c = GetCharPressed();
		}
	}

	if (leftClickedWidget != nullptr) {
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			leftClickedWidget->get()->leftMouseReleased();
		}
	}
}

void Gui::draw() {
	ClearBackground(background);
	for (auto &widget : widgets) {
		widget->draw();
	}
	for (auto &widget : topLayer) {
		widget->draw();
	}
}

void Gui::add(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this;
	}
	widget->render->font = &this->font;
	widget->unfocused();
	widgets.push_back(widget);
}

void Gui::addTop(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this;
	}
	widget->render->font = &this->font;
	widget->unfocused();
	topLayer.push_back(widget);
}

void Gui::notifyChild(std::shared_ptr<Widget> *widget) {
	if (!widget->get()->notifiedMouseEnter) {
		widget->get()->mouseEntered();
		widget->get()->notifiedMouseEnter = true;
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		widget->get()->leftMouseClicked();
		leftClickedWidget = widget;

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
}

Rectangle Gui::getScreenRect() {
	return {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
}

void Gui::setFont(const char *fileName, int fontSize) {
	if (IsFontValid(font)) {
		UnloadFont(font);
	}

	auto codepoints = loadFontCodepoints();
	font = LoadFontEx(fileName, fontSize, codepoints.data(), codepoints.size());
}

void Gui::unload() {
	if (IsFontValid(font)) {
		UnloadFont(font);
	}
}
