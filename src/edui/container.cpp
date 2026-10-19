#include "edui/container.hpp"

#include <memory>

#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

Container::Container() {
	this->isContainer = true;
	this->focusable = false;
	render = std::make_unique<ContainerRender>();
	gui = Gui::instance;
}

void Container::applyTheme(const mINI::INIStructure &struc) {
	Widget::applyTheme(struc);
	for (auto &widget : widgets) {
		widget->as<edui::Widget>().applyTheme(struc);
		widget->unfocused();
	}
}

void Container::translate() {
	for (auto &widget : widgets) {
		widget->translate();
	}
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

		widget->calcRect(renderRect);
		widget->update();

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

	drawChildren();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void Container::drawChildren() {
	for (auto &widget : widgets) {
		if (widget->visible && !widget->deleteFlag) {
			widget->draw();
		}
	}
}

void Container::add(std::shared_ptr<Widget> widget) {
	if (widget->isContainer) {
		widget->as<Container>().gui = this->gui;
	}

	widget->render->font = &gui->font;
	widget->unfocused();
	widget->layerId = layerId;
	widgets.push_back(widget);
	widget->applyTheme(Gui::instance->themeStruct);
	widget->onAdded();
}

void Container::notifyChildren(Gui *gui) {
	for (auto &widget : widgets) {
		if (widget->mouseIsInRect() && mouseIsInContent()) {
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

std::shared_ptr<edui::Widget> Container::getWidgetById(int referId) {
	std::shared_ptr<edui::Widget> result = widgets.front();
	for (auto &widget : widgets) {
		if (widget->referId == referId) {
			result = widget;
			return result;
		}
	}
	printf("[EDUI] widget not found\n");
	return result;
}

void Container::markDelete() {
	for (auto &widget : widgets) {
		widget->markDelete();
	}
	Widget::markDelete();
}

void Container::deleteChildren() {
	for (auto &widget : widgets) {
		if (widget != nullptr) {
			if (widget->isContainer) {
				widget->as<Container>().deleteChildren();
			}
			widget->markDelete();
		}
	}
	widgets.clear();
}
