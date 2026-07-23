#include "edui/verticalContainer.hpp"
#include <cmath>

using namespace edui;

VerticalContainer::VerticalContainer() {
	this->isContainer = true;
	render = std::make_unique<VerticalContainerRender>();
}

void VerticalContainer::add(std::shared_ptr<Widget> widget) {
	auto& rend = render->as<VerticalContainerRender>();

	int count = widgets.size();

	int res = 0;
	for (auto& widget : widgets) {
		res += (widget->layout.height.offset) + rend.space;
	}
	int widgetWidth = widget->layout.height.offset;

	widget->layout.x = {0, 0};
	widget->layout.y = {0, res};
	widget->layout.height = {0, widgetWidth};
	widget->layout.width = {1.0f, 0};

	this->scissorRect.height = ((rend.padding * 2) + res + widgetWidth);

	Container::add(widget);
}

void VerticalContainer::scrolled(float mouseWheel) {
	float added = (mouseWheel * 6);
	float content = contentRect.height;
	float scissor = scissorRect.height;

	if ((scissorY + added) >= 0.0f) {
		scissorY = 0;
		return;
	}

	if ((scissorY + added) < (-scissor + content)) {
		scissorY = (-scissor + content);
		return;
	}

	scissorY += added;
}
