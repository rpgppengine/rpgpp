#include "edui/container.hpp"

#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

Container::Container() {
	this->isContainer = true;
	this->focusable = false;
	render = std::make_unique<ContainerRender>();
}

void Container::update() {
	updateContentRect();

	for (auto &widget : widgets) {
		widget->update();
		widget->calcRect(renderRect);

		if (!widget->mouseIsInRect() || !mouseIsInRect()) {
			if (widget->notifiedMouseEnter) {
				widget->notifiedMouseEnter = false;
				widget->mouseLeft();
			}
		}
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
	auto &rend = render->as<ContainerRender>();

	DrawRectangleRec(rect, rend.bgColor);

	if (isScissor) {
		BeginScissorMode(contentRect.x, contentRect.y, contentRect.width, contentRect.height);
	}

	for (auto &widget : widgets) {
		widget->draw();
	}

	if (isScissor) EndScissorMode();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void Container::add(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this->gui;
	}
	widget->render->font = &gui->font;
	widget->unfocused();
	widgets.push_back(widget);
}

void Container::notifyChildren(Gui *gui) {
	for (auto &widget : widgets) {
		if (widget->mouseIsInRect()) {
			if (!widget->isContainer) {
				gui->notifyChild(&widget);
			} else {
				widget->scrolled(GetMouseWheelMove());
				gui->notifyChild(&widget);
				widget->as<Container>().notifyChildren(gui);
			}
		}
	}
}

void Container::mouseEntered() {
	render->as<ContainerRender>().currentScrollbarColor = render->as<ContainerRender>().focusScrollbarColor;
}

void Container::mouseLeft() {
	render->as<ContainerRender>().currentScrollbarColor = render->as<ContainerRender>().scrollbarColor;
}

void Container::markDelete() {
	for (auto &widget : widgets) {
		widget->markDelete();
	}
	Widget::markDelete();
}
