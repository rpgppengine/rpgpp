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

			if (widget->deleteOnOutsideClick) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					widget->markDelete();
				}
			}
		}

		widget->update();
		widget->calcRect(screenRect);

		if (leftClickedWidget != nullptr) {
			if (leftClickedWidget->get()->deleteFlag) {
				leftClickedWidget = nullptr;
			}
		}

		if (widget->deleteFlag) {
			widget->onDeleted.invoke();
			vec.erase(vec.begin() + i);
		}

		widget->deferFlag = true;

		i++;
	}
}

void Gui::update() {
	screenRect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};

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

	if (leftClickedWidget != nullptr) {
		if (!leftClickedWidget->get()->deleteFlag) {
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				leftClickedWidget->get()->leftMouseReleased();
			}
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

	if (!FloatEquals(GetMouseDelta().x, 0.0f)) {
		auto mousePos = GetMousePosition();
		auto relative = mousePos;
		relative.x -= widget->get()->rect.x;
		relative.y -= widget->get()->rect.y;
		widget->get()->mouseMoved(GetMousePosition(), relative);
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		leftClickedWidget = widget;
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
