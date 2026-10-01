#include "edui/verticalScrollbar.hpp"

#include <memory>

#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

VerticalScrollbar::VerticalScrollbar() : Widget() { render = std::make_unique<VerticalScrollbarRender>(); }

void VerticalScrollbar::setMaxScroll(int newValue) { this->maxScroll = newValue; }

void VerticalScrollbar::setValue(int value) { this->currentValue = value; }

void VerticalScrollbar::update() {
	if (scrolling) {
		setValueFromMouse();
	}
}

void VerticalScrollbar::draw() {
	auto &rend = render->as<VerticalScrollbarRender>();

	DrawRectangleRec(rect, rend.bgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.borderColor);

	drawBar();
}

Rectangle VerticalScrollbar::getBarRect() {
	float scrollbarSize = calcScrollbarSize();
	float valueFraction = static_cast<float>(currentValue) / maxScroll;
	float barOffset = valueFraction * (rect.height - scrollbarSize);

	Rectangle barRect = {rect.x, rect.y + barOffset, rect.width, scrollbarSize};
	return barRect;
}

void VerticalScrollbar::drawBar() {
	auto &rend = render->as<VerticalScrollbarRender>();

	Rectangle barRect = getBarRect();
	DrawRectangleRec(barRect, rend.currentBgColor);
	DrawRectangleLinesEx(barRect, rend.border, rend.currentBorderColor);
}

void VerticalScrollbar::setValueFromMouse() {
	float scrollbarSize = calcScrollbarSize();
	float maxRelativeY = (rect.height - scrollbarSize);

	auto barRect = getBarRect();
	barRect.y = GetMousePosition().y - mouseOffset;

	float resFraction = (barRect.y - rect.y) / maxRelativeY;
	int result = resFraction * maxScroll;
	result = constrain(result, 0, maxScroll);

	if (currentValue != result) {
		onValueChanged.invoke(result);
	}

	this->currentValue = result;
}

float VerticalScrollbar::calcScrollbarSize() {
	float scrollbarSize = (maxScroll / rect.height) * rect.height;
	if (rect.height < maxScroll) {
		scrollbarSize = (rect.height / maxScroll) * rect.height;
	}

	if (alternativeCalc) {
		scrollbarSize = (rect.height / maxContent) * rect.height;
	}

	return scrollbarSize;
}

void VerticalScrollbar::leftMouseClicked() {
	Widget::leftMouseClicked();

	auto barRect = getBarRect();
	if (!CheckCollisionPointRec(GetMousePosition(), barRect)) {
		mouseOffset = barRect.height / 2;
		setValueFromMouse();
		barRect = getBarRect();
	}

	Vector2 barPos = {barRect.x, barRect.y};
	Vector2 offsetVec = Vector2Subtract(GetMousePosition(), barPos);
	mouseOffset = static_cast<int>(offsetVec.y);

	scrolling = true;
}

void VerticalScrollbar::leftMouseReleased() {
	Widget::leftMouseReleased();
	scrolling = false;
}
