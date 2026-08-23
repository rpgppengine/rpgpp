#include "edui/slider.hpp"

#include <memory>

#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

Slider::Slider() { render = std::make_unique<SliderRender>(); }

void Slider::setMinValue(int newMinValue) {
	this->minValue = newMinValue;
	value = constrain(value, minValue, maxValue);
}

void Slider::setMaxValue(int newMaxValue) {
	this->maxValue = newMaxValue;
	value = constrain(value, minValue, maxValue);
}

Rectangle Slider::getSliderRect() {
	float sliderMaxPos = rect.x + (rect.width - EDUI_SLIDER_WIDTH);

	float sliderPos = rect.x + ((sliderMaxPos - rect.x) / (maxValue - minValue)) * (value - minValue);

	Rectangle sliderRect = {sliderPos, rect.y, EDUI_SLIDER_WIDTH, rect.height};

	return sliderRect;
}

void Slider::setValueByMouse() {
	auto sliderRect = getSliderRect();

	if (barOffset.x < 0) {
		barOffset.x = (EDUI_SLIDER_WIDTH / 2);
	}

	auto mousePos = GetMousePosition();

	float sliderMaxPos = rect.x + (rect.width - EDUI_SLIDER_WIDTH);
	float newSliderPos = mousePos.x - barOffset.x;

	float newValue = minValue + ((maxValue - minValue) / (sliderMaxPos - rect.x)) * (newSliderPos - rect.x);
	newValue = constrain(newValue, static_cast<float>(minValue), static_cast<float>(maxValue));

	if (this->value != newValue) {
		this->value = newValue;
		onValueChanged.invoke(newValue);
	}
}

void Slider::draw() {
	auto &rend = render->as<SliderRender>();

	auto sliderRect = getSliderRect();

	DrawRectangleRec(sliderRect, rend.currentBgColor);
	DrawRectangleLinesEx(sliderRect, rend.border, rend.currentBorderColor);

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void Slider::leftMouseClicked() {
	holdingMouse = true;
	auto sliderRect = getSliderRect();
	if (CheckCollisionPointRec(GetMousePosition(), sliderRect)) {
		auto offset = Vector2Subtract(GetMousePosition(), {sliderRect.x, sliderRect.y});
		barOffset.x = offset.x;
	}
}

void Slider::leftMouseReleased() {
	holdingMouse = false;

	setValueByMouse();
}

void Slider::mouseMoved(Vector2 mousePos, Vector2 relative) {
	if (holdingMouse) {
		setValueByMouse();
	}
}
