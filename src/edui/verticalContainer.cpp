#include "edui/verticalContainer.hpp"

#include <cmath>
#include <memory>

#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/scrollableContainer.hpp"
#include "edui/verticalScrollbar.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

VerticalContainer::VerticalContainer() {
	this->isContainer = true;
	render = std::make_unique<VerticalContainerRender>();

	scrollbar = std::make_shared<edui::VerticalScrollbar>();
	scrollbar->setPosition({1, static_cast<int>(-EDUI_DEFAULT_HEIGHT)}, {0, 0});
	scrollbar->setSize({0, static_cast<int>(EDUI_DEFAULT_HEIGHT)}, {1, 0});

	scrollbar->onValueChanged.connect([this](int newValue) { this->scissorY = -newValue; });
}

void VerticalContainer::applyTheme(const mINI::INIStructure &struc) {
	ScrollableContainer::applyTheme(struc);
	scrollbar->applyTheme(struc);
	scrollbar->unfocused();
}

void VerticalContainer::update() {
	ScrollableContainer::update();

	scrollbar->calcRect(rect);
	scrollbar->update();

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

void VerticalContainer::draw() {
	ScrollableContainer::draw();
	if (overflown) {
		scrollbar->draw();
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

	ScrollableContainer::add(widget);
}

void VerticalContainer::updateContentRect() {
	this->contentRect = rect;
	if (overflown) {
		float old = contentRect.width;
		contentRect.width = old - EDUI_DEFAULT_HEIGHT;
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	float content = contentRect.height;
	float scissor = scissorRect.height;
	scrollMax = (-scissor + content);
	scrollbar->setMaxScroll(-scrollMax);

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
		scrollbar->setValue(-scissorY);
		return;
	}

	if ((scissorY + added) < scrollMax) {
		scissorY = scrollMax;
		scrollbar->setValue(-scissorY);
		return;
	}

	scissorY += added;
	scrollbar->setValue(-scissorY);
}
