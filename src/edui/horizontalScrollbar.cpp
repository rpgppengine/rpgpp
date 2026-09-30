#include "edui/horizontalScrollbar.hpp"

#include <memory>

#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

HorizontalScrollbar::HorizontalScrollbar() : Widget() { render = std::make_unique<HorizontalScrollbarRender>(); }

void HorizontalScrollbar::setMaxScroll(int newValue) { this->maxScroll = newValue; }

void HorizontalScrollbar::setValue(int value) { this->currentValue = value; }

void HorizontalScrollbar::update() {
	if (scrolling) {
		setValueFromMouse();
	}
}

void HorizontalScrollbar::draw() {
	auto &rend = render->as<HorizontalScrollbarRender>();

	DrawRectangleRec(rect, rend.bgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.borderColor);

	drawBar();
}

Rectangle HorizontalScrollbar::getBarRect() {
	float scrollbarSize = (maxScroll / rect.width) * maxScroll;
	if (rect.width < maxScroll) {
		scrollbarSize = (rect.width / maxScroll) * rect.width;
	}
	float valueFraction = static_cast<float>(currentValue) / maxScroll;
	float barOffset = valueFraction * (rect.width - scrollbarSize);

	Rectangle barRect = {rect.x + barOffset, rect.y, scrollbarSize, rect.height};
	return barRect;
}

void HorizontalScrollbar::drawBar() {
	auto &rend = render->as<HorizontalScrollbarRender>();

	auto barRect = getBarRect();
	DrawRectangleRec(barRect, rend.currentBgColor);
	DrawRectangleLinesEx(barRect, rend.border, rend.currentBorderColor);
}

void HorizontalScrollbar::setValueFromMouse() {
	float scrollbarSize = (maxScroll / rect.width) * maxScroll;
	if (rect.width < maxScroll) {
		scrollbarSize = (rect.width / maxScroll) * rect.width;
	}
	float maxRelativeX = (rect.width - scrollbarSize);

	auto barRect = getBarRect();
	barRect.x = GetMousePosition().x - mouseOffset;

	float resFraction = (barRect.x - rect.x) / maxRelativeX;
	int result = resFraction * maxScroll;
	result = constrain(result, 0, maxScroll);

	if (currentValue != result) {
		onValueChanged.invoke(result);
	}

	this->currentValue = result;
}

void HorizontalScrollbar::leftMouseClicked() {
	Widget::leftMouseClicked();
	auto barRect = getBarRect();
	if (!CheckCollisionPointRec(GetMousePosition(), barRect)) {
		mouseOffset = barRect.width / 2;
		setValueFromMouse();
		barRect = getBarRect();
	}

	Vector2 barPos = {barRect.x, barRect.y};
	Vector2 offsetVec = Vector2Subtract(GetMousePosition(), barPos);
	mouseOffset = static_cast<int>(offsetVec.x);

	scrolling = true;
}

void HorizontalScrollbar::leftMouseReleased() {
	Widget::leftMouseReleased();
	scrolling = false;
}
