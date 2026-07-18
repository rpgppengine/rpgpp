#include "ui_elements/uiElement.hpp"

#include "game.hpp"

UIElement::UIElement() { init(); }

UIElement::UIElement(const std::string &typeName) {
	this->typeName = typeName;
	init();
}

void UIElement::init() {
	props["rect"] = Rectangle{0, 0, 20, 20};
	props["visible"] = true;

	callbacks[CALLBACK_FOCUSED] = [] {};
	callbacks[CALLBACK_UNFOCUSED] = [] {};
	callbacks[CALLBACK_TRIGGER] = [] {};
}

void UIElement::config() {}

void UIElement::onNotify(Event event) {
	if (props.count("input") <= 0) return;
	if (event.hold) return;

	InputC input = std::get<InputC>(props["input"]);

	if (event.key == KEY_UP) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.upButton.title);
	}
	if (event.key == KEY_DOWN) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.downButton.title);
	}
	if (event.key == KEY_LEFT) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.leftButton.title);
	}
	if (event.key == KEY_RIGHT) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.rightButton.title);
	}
	if (event.key == KEY_Z) {
		if (callbacks[CALLBACK_TRIGGER] != nullptr) {
			callbacks[CALLBACK_TRIGGER]();
		}

		auto *view = Game::getUi().getCurrentView();
		if (view->hasScript()) {
			auto luaFunc = view->getLuaEnvironment()[input.funcNames.funcNames[CALLBACK_TRIGGER]];
			if (luaFunc.is<sol::function>()) {
				luaFunc();
			}
		}
	}
}
