#include "edui/verticalContainer.hpp"

#include <cmath>

#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

VerticalContainer::VerticalContainer() {
	this->isContainer = true;
	render = std::make_unique<VerticalContainerRender>();
}

void VerticalContainer::update() {
	if (scrolling) {
		float lowerLimit = scrollOffset.y;
		float upperLimit = scrollAreaRect.height - (scrollbarRect.height - scrollOffset.y);

		Vector2 offset = Vector2Subtract(GetMousePosition(), {scrollbarRect.x, scrollAreaRect.y + lowerLimit});
		float newOffsetY = constrain<float>(offset.y, 0, static_cast<float>(upperLimit - lowerLimit));
		offset.y = newOffsetY;

		float max = upperLimit - lowerLimit;
		float fract = (offset.y / max);
		scissorY = (fract * scrollMax);
	}
	Container::update();
}

void VerticalContainer::draw() {
	Container::draw();
	if (overflown) {
		DrawRectangleRec(scrollbarRect, render->as<ContainerRender>().currentScrollbarColor);
		DrawRectangleLinesEx(scrollAreaRect, 1.0f, render->currentBorderColor);
	}
}

void VerticalContainer::add(std::shared_ptr<Widget> widget) {
	auto &rend = render->as<VerticalContainerRender>();

	int count = widgets.size();

	int res = 0;
	for (auto &widget : widgets) {
		res += (widget->layout.height.offset) + rend.space;
	}
	int widgetHeight = widget->layout.height.offset;

	widget->layout.x = {0, 0};
	widget->layout.y = {0, res};
	widget->layout.height = {0, widgetHeight};
	widget->layout.width = {1.0f, 0};

	this->scissorRect.height = ((rend.padding * 2) + res + widgetHeight);

	Container::add(widget);
}

void VerticalContainer::updateContentRect() {
	this->contentRect = rect;
	if (overflown) {
		float old = contentRect.width;
		contentRect.width = old - ScrollbarSize;
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	scrollAreaRect = {rect.x, rect.y, ScrollbarSize, rect.height};
	scrollAreaRect.x += rect.width - ScrollbarSize;

	this->scrollbarHeight = (contentRect.height / scissorRect.height) * contentRect.height;

	float fract = (-scissorY / -scrollMax);
	this->scrollbarRect = {scrollAreaRect.x, scrollAreaRect.y, ScrollbarSize, scrollbarHeight};
	scrollbarRect.y -= fract * (scrollbarHeight - contentRect.height);

	float content = contentRect.height;
	float scissor = scissorRect.height;
	scrollMax = (-scissor + content);

	if (scissorRect.height > contentRect.height) {
		overflown = true;
	} else {
		overflown = false;
	}
}

void VerticalContainer::scrolled(float mouseWheel) {
	if (!overflown) return;

	float added = (mouseWheel * ScrollSpeed);

	if ((scissorY + added) >= 0.0f) {
		scissorY = 0;
		return;
	}

	if ((scissorY + added) < scrollMax) {
		scissorY = scrollMax;
		return;
	}

	scissorY += added;
}

void VerticalContainer::leftMouseClicked() {
	if (CheckCollisionPointRec(GetMousePosition(), scrollAreaRect)) {
		Vector2 offset = Vector2Subtract(GetMousePosition(), {scrollbarRect.x, scrollbarRect.y});
		if (!CheckCollisionPointRec(GetMousePosition(), scrollbarRect)) {
			offset.y = scrollbarRect.height / 2.0f;
		}
		this->scrollOffset = offset;
		scrolling = true;
	}
}

void VerticalContainer::leftMouseReleased() {
	scrolling = false;
}
