#include "edui/horizontalContainer.hpp"

#include <memory>

#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "edui/scrollableContainer.hpp"
#include "raymath.h"

using namespace edui;

HorizontalContainer::HorizontalContainer() {
	this->isContainer = true;
	render = std::make_unique<HorizontalContainerRender>();
}

void HorizontalContainer::update() {
	if (scrolling) {
		float lowerLimit = scrollOffset.x;
		float upperLimit = scrollAreaRect.width - (scrollbarRect.width - scrollOffset.x);

		Vector2 offset = Vector2Subtract(GetMousePosition(), {scrollAreaRect.x + lowerLimit, scrollbarRect.y});
		float newOffsetX = constrain<float>(offset.x, 0, static_cast<float>(upperLimit - lowerLimit));
		offset.x = newOffsetX;

		float max = upperLimit - lowerLimit;
		float fract = (offset.x / max);
		scissorX = (fract * scrollMax);
	}
	ScrollableContainer::update();
}

void HorizontalContainer::draw() {
	ScrollableContainer::draw();
	if (overflown) {
		DrawRectangleRec(scrollbarRect, render->as<HorizontalContainerRender>().currentScrollbarColor);
		DrawRectangleLinesEx(scrollAreaRect, 1.0f, render->currentBorderColor);
	}
}

void HorizontalContainer::add(std::shared_ptr<Widget> widget) {
	auto &rend = render->as<HorizontalContainerRender>();

	int count = widgets.size();

	int res = 0;
	for (auto &widget : widgets) {
		res += (widget->layout.width.offset) + rend.space;
	}
	int widgetWidth = widget->layout.width.offset;

	widget->layout.x = {0, res};
	widget->layout.y = {0, 0};
	widget->layout.height = {1.0f, 0};
	widget->layout.width = {0, widgetWidth};

	this->scissorRect.width = ((rend.padding * 2) + res + widgetWidth);

	ScrollableContainer::add(widget);
}

void HorizontalContainer::updateContentRect() {
	this->contentRect = rect;
	if (overflown) {
		float old = contentRect.height;
		contentRect.height = old - ScrollbarSize;
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	scrollAreaRect = {rect.x, rect.y, rect.width, ScrollbarSize};
	scrollAreaRect.y += rect.height - ScrollbarSize;

	this->scrollbarHeight = (contentRect.width / scissorRect.width) * contentRect.width;

	float fract = (-scissorX / -scrollMax);
	this->scrollbarRect = {scrollAreaRect.x, scrollAreaRect.y, scrollbarHeight, ScrollbarSize};
	scrollbarRect.x -= fract * (scrollbarHeight - contentRect.width);

	float content = contentRect.width;
	float scissor = scissorRect.width;
	scrollMax = (-scissor + content);

	if (scissorRect.width > contentRect.width) {
		overflown = true;
	} else {
		overflown = false;
	}
}

void HorizontalContainer::scrolled(float mouseWheel) {
	float added = (mouseWheel * ScrollSpeed);

	if ((scissorX + added) >= 0.0f) {
		scissorX = 0;
		return;
	}

	if ((scissorX + added) < scrollMax) {
		scissorX = scrollMax;
		return;
	}

	scissorX += added;
}

void HorizontalContainer::leftMouseClicked() {
	if (CheckCollisionPointRec(GetMousePosition(), scrollAreaRect)) {
		Vector2 offset = Vector2Subtract(GetMousePosition(), {scrollbarRect.x, scrollbarRect.y});
		if (!CheckCollisionPointRec(GetMousePosition(), scrollbarRect)) {
			offset.x = scrollbarRect.width / 2.0f;
		}
		this->scrollOffset = offset;
		scrolling = true;
	}
}

void HorizontalContainer::leftMouseReleased() {
	scrolling = false;
}
