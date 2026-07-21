#include "edui/container.hpp"
#include "edui/helper.hpp"

using namespace edui;

Container::Container() {
	this->isContainer = true;
	render = std::make_unique<ContainerRender>();
}

void Container::update() {
	updateRenderRect();

	for (auto& widget : widgets) {
		widget->update();
		widget->calcRect(renderRect);
	}
}

void Container::updateRenderRect() {
	this->renderRect = paddingRect(rect, render->padding);
}

void Container::draw() {
	auto& rend = render->as<ContainerRender>();

	DrawRectangleRec(rect, rend.bgColor);
	DrawRectangleLinesEx(rect, rend.border, BLACK);

	for (auto& widget : widgets) {
		widget->draw();
	}
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
