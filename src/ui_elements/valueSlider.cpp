#include "ui_elements/valueSlider.hpp"

ValueSlider::ValueSlider() : UIElement("ValueSlider") { init(); }

void ValueSlider::init() {
	props["value"] = 0;
	props["minValue"] = 0;
	props["maxValue"] = 100;
	props["normalColor"] = RAYWHITE;
	props["focusedColor"] = GRAY;
	props["input"] = InputC{};

	callbacks[CALLBACK_FOCUSED] = [this] {
		auto focusedTextColor = std::get<Color>(props["focusedColor"]);
		currentColor = focusedTextColor;
	};
	callbacks[CALLBACK_UNFOCUSED] = [this] {
		auto normalTextColor = std::get<Color>(props["normalColor"]);
		currentColor = normalTextColor;
	};
}

void ValueSlider::config() {
	Color normalColor = std::get<Color>(props["normalColor"]);
	this->currentColor = normalColor;
}

void ValueSlider::update() {}

void ValueSlider::draw(Rectangle rect) {
	int value = std::get<int>(props["value"]);
	int minValue = std::get<int>(props["minValue"]);
	int maxValue = std::get<int>(props["maxValue"]);

	Rectangle filledRect = rect;
	filledRect.width *= (static_cast<float>(value) / maxValue);

	DrawRectangleRec(filledRect, currentColor);
}

void ValueSlider::onNotify(Event event) {
	int *valuePtr = std::get_if<int>(&props["value"]);
	int minValue = std::get<int>(props["minValue"]);
	int maxValue = std::get<int>(props["maxValue"]);

	if (event.key == KEY_LEFT) {
		if (*valuePtr > minValue) {
			int val = *valuePtr;
			val--;
			*valuePtr = val;
		}
	}
	if (event.key == KEY_RIGHT) {
		if (*valuePtr < maxValue) {
			int val = *valuePtr;
			val++;
			*valuePtr = val;
		}
	}

	UIElement::onNotify(event);
}
