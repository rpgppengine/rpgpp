#include "edui/horizontalContainer.hpp"

#include <memory>

#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/horizontalScrollbar.hpp"
#include "edui/scrollableContainer.hpp"
#include "raymath.h"

using namespace edui;

HorizontalContainer::HorizontalContainer() {
	this->isContainer = true;
	render = std::make_unique<HorizontalContainerRender>();

	scrollbar = std::make_shared<HorizontalScrollbar>();
	scrollbar->setPosition({0, 0}, {1, static_cast<int>(-EDUI_DEFAULT_HEIGHT)});
	scrollbar->setSize({1, 0}, {0, static_cast<int>(EDUI_DEFAULT_HEIGHT)});

	scrollbar->onValueChanged.connect([this](int newValue) { this->scissorX = -newValue; });
}

void HorizontalContainer::applyTheme(const mINI::INIStructure &struc) {
	ScrollableContainer::applyTheme(struc);
	scrollbar->applyTheme(struc);
	scrollbar->unfocused();
}

void HorizontalContainer::update() {
	ScrollableContainer::update();

	scrollbar->calcRect(rect);
	scrollbar->update();

	if (scrollbar->visible) {
		if (scrollbar->mouseIsInRect()) {
			std::shared_ptr<Widget> widgetPtr = scrollbar;
			Gui::instance->notifyChild(&widgetPtr);
		} else {
			if (scrollbar->notifiedMouseEnter) {
				scrollbar->notifiedMouseEnter = false;
				scrollbar->mouseLeft();
			}
		}
	}

	auto &rend = render->as<HorizontalContainerRender>();

	/*
	this->scissorRect.width = 0;
	for (auto &widget : widgets) {
		if (widget->visible && !widget->deleteFlag) {
			widget->layout.x.offset = this->scissorRect.width;
			this->scissorRect.width += widget->rect.width;

			if (reverse) {
				widget->layout.x.offset = rect.width - this->scissorRect.width;
			}

			this->scissorRect.width += rend.space;
		}
	}
	*/
}

void HorizontalContainer::draw() {
	ScrollableContainer::draw();

	if (overflown && scrollbar->visible) {
		scrollbar->draw();
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
	if (reverse) {
		widget->layout.x = {1, static_cast<int>(-(widgetWidth + res))};
	}
	widget->layout.y = {0, 0};
	widget->layout.height = {1.0f, 0};
	widget->layout.width = {0, widgetWidth};

	this->scissorRect.width = ((rend.padding * 2) + res + widgetWidth);

	/*
	this->scissorRect.width = ((rend.padding * 2) + res + widgetWidth);

	int size = widgets.size();

	widget->onDeleted = [this, size, widgetWidth] {
		deferFlag = true;
		for (int i = (size - 1); i < widgets.size(); i++) {
			widgets.at(i)->layout.x.offset -= widgetWidth;
		}
		this->scissorRect.width -= widgetWidth;
	};
	*/

	ScrollableContainer::add(widget);
}

void HorizontalContainer::updateContentRect() {
	this->contentRect = rect;
	if (overflown) {
		float old = contentRect.height;
		contentRect.height = old - EDUI_DEFAULT_HEIGHT;
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	float content = contentRect.width;
	float scissor = scissorRect.width;
	scrollMax = (-scissor + content);
	scrollbar->setMaxScroll(-scrollMax);

	if (scissorRect.width > contentRect.width) {
		overflown = true;
	} else {
		overflown = false;
	}
}

void HorizontalContainer::scrolled(float mouseWheel) {
	if (!overflown) return;

	float added = (mouseWheel * ScrollSpeed);

	if ((scissorX + added) >= 0.0f) {
		scissorX = 0;
		scrollbar->setValue(-scissorX);
		return;
	}

	if ((scissorX + added) < scrollMax) {
		scissorX = scrollMax;
		scrollbar->setValue(-scissorX);
		return;
	}

	scissorX += added;
	scrollbar->setValue(-scissorX);
}

void HorizontalContainer::leftMouseClicked() {
	if (!overflown) return;
	if (CheckCollisionPointRec(GetMousePosition(), scrollAreaRect)) {
		Vector2 offset = Vector2Subtract(GetMousePosition(), {scrollbarRect.x, scrollbarRect.y});
		if (!CheckCollisionPointRec(GetMousePosition(), scrollbarRect)) {
			offset.x = scrollbarRect.width / 2.0f;
		}
		this->scrollOffset = offset;
		scrolling = true;
	}
}

void HorizontalContainer::leftMouseReleased() { scrolling = false; }

void HorizontalContainer::updateWidgets() {
	auto &rend = render->as<HorizontalContainerRender>();

	int res = 0;
	for (auto &widget : widgets) {
		if (!widget->deleteFlag) {
			int widgetWidth = widget->layout.width.offset;

			widget->layout.x = {0, res};
			if (reverse) {
				widget->layout.x = {1, static_cast<int>(-(widgetWidth + res))};
			}

			this->scissorRect.width = ((rend.padding * 2) + res + widgetWidth);

			res += (widget->layout.width.offset) + rend.space;
		}
	}
}

void HorizontalContainer::fixScroll() {
	if (scissorX >= 0.0f) {
		scissorX = 0;
		scrollbar->setValue(-scissorX);
		return;
	}

	if (scissorX < scrollMax) {
		scissorX = scrollMax;
		scrollbar->setValue(-scissorX);
		return;
	}

	scrollbar->setValue(-scissorX);
}
