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

	int i = 0;
	for (auto &widget : widgets) {
		if (widget == nullptr) continue;

		if (!widget->visible) continue;

		if (widget->deferFlag) {
			widget->deferFlag = false;
			continue;
		}

		widget->update();
		widget->calcRect(renderRect);

		if (!widget->mouseIsInRect() || !mouseIsInContent()) {
			if (widget->notifiedMouseEnter) {
				widget->notifiedMouseEnter = false;
				widget->mouseLeft();
			}
		}

		if (widget->deleteFlag) {
			widget->onDeleted.invoke();
			widgets.erase(widgets.begin() + i);
		}

		i++;
	}
}

void Container::updateContentRect() {
	this->contentRect = getContentRect();
	renderRect = paddingRect(contentRect, render->padding);
}

void Container::draw() {
	auto &rend = render->as<ContainerRender>();

	DrawRectangleRec(rect, rend.bgColor);

	for (auto &widget : widgets) {
		if (widget->visible) {
			widget->draw();
		}
	}

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

void Container::markDelete() {
	for (auto &widget : widgets) {
		widget->markDelete();
	}
	Widget::markDelete();
}
