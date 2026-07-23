#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "raylib.h"

using namespace edui;

Container::Container() {
	this->isContainer = true;
	render = std::make_unique<ContainerRender>();
}

void Container::update() {
	updateContentRect();

	for (auto& widget : widgets) {
		widget->update();
		widget->calcRect(renderRect);
	}
}

void Container::updateContentRect() {
	this->contentRect = rect;
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}
}

void Container::draw() {
	auto& rend = render->as<ContainerRender>();

	DrawRectangleRec(rect, rend.bgColor);

	if (isScissor) {
		BeginScissorMode(contentRect.x, contentRect.y, contentRect.width, contentRect.height);
	}

	for (auto& widget : widgets) {
		widget->draw();
	}

	if (isScissor) EndScissorMode();

	DrawRectangleLinesEx(rect, rend.border, BLACK);
}

void Container::add(std::shared_ptr<Widget> widget) {
	widget->unfocused();
	widgets.push_back(widget);
}

void Container::notifyChildren(Gui* gui) {
	for (auto& widget : widgets) {
		if (widget->mouseIsInRect()) {
			gui->notifyChild(&widget);
		}
	}
}
