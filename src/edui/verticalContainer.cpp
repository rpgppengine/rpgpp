#include "edui/verticalContainer.hpp"
#include <cmath>
#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "raylib.h"

using namespace edui;

VerticalContainer::VerticalContainer() {
	this->isContainer = true;
	render = std::make_unique<VerticalContainerRender>();
}

void VerticalContainer::draw() {
	Container::draw();
	if (overflown) {
		float flipped = -scissorY;
		float fract = (flipped / -scrollMax);

		Rectangle scrollbar = {scrollAreaRect.x, scrollAreaRect.y, ScrollbarSize, scrollbarHeight};
		scrollbar.y -= fract * (scrollbarHeight - contentRect.height);
		DrawRectangleRec(scrollbar, GRAY);

		DrawRectangleLinesEx(scrollAreaRect, 1.0f, render->borderColor);
	}
}

void VerticalContainer::add(std::shared_ptr<Widget> widget) {
	auto& rend = render->as<VerticalContainerRender>();

	int count = widgets.size();

	int res = 0;
	for (auto& widget : widgets) {
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
		contentRect.width  = old - ScrollbarSize;
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	scrollAreaRect = {rect.x, rect.y, ScrollbarSize, rect.height};
	scrollAreaRect.x += rect.width - ScrollbarSize;

	this->scrollbarHeight = (contentRect.height / scissorRect.height) * contentRect.height;

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
