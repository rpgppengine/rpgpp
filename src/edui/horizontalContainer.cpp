#include "edui/horizontalContainer.hpp"
#include <memory>
#include "edui/container.hpp"
#include "edui/helper.hpp"

using namespace edui;

HorizontalContainer::HorizontalContainer() {
	this->isContainer = true;
	render = std::make_unique<HorizontalContainerRender>();
}

void HorizontalContainer::draw() {
	Container::draw();
	if (overflown) {
		float flipped = -scissorX;
		float fract = (flipped / -scrollMax);

		Rectangle scrollbar = {scrollAreaRect.x, scrollAreaRect.y, scrollbarHeight, ScrollbarSize};
		scrollbar.x -= fract * (scrollbarHeight - contentRect.width);
		DrawRectangleRec(scrollbar, GRAY);

		DrawRectangleLinesEx(scrollAreaRect, 1.0f, render->borderColor);
	}
}

void HorizontalContainer::add(std::shared_ptr<Widget> widget) {
	auto& rend = render->as<HorizontalContainerRender>();

	int count = widgets.size();

	int res = 0;
	for (auto& widget : widgets) {
		res += (widget->layout.width.offset) + rend.space;
	}
	int widgetWidth = widget->layout.width.offset;

	widget->layout.x = {0, res};
	widget->layout.y = {0, 0};
	widget->layout.height = {1.0f, 0};
	widget->layout.width = {0, widgetWidth};

	updateContentRect();
	this->scissorRect.width = ((rend.padding * 2) + res + widgetWidth);

	Container::add(widget);
}

void HorizontalContainer::updateContentRect() {
	this->contentRect = rect;
	if (overflown) {
		float old = contentRect.width;
		contentRect.height  = old - ScrollbarSize;
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	scrollAreaRect = {rect.x, rect.y + (rect.height - ScrollbarSize), rect.width, ScrollbarSize};

	this->scrollbarHeight = (contentRect.width / scissorRect.width) * contentRect.width;

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
	float content = contentRect.width;
	float scissor = scissorRect.width;

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
