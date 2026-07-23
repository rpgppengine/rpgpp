#include "edui/horizontalContainer.hpp"
#include <memory>
#include "edui/container.hpp"

using namespace edui;

HorizontalContainer::HorizontalContainer() {
	this->isContainer = true;
	render = std::make_unique<HorizontalContainerRender>();
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

	this->scissorRect.width = res + widgetWidth;

	Container::add(widget);
}

void HorizontalContainer::scrolled(float mouseWheel) {
	float added = (mouseWheel * 6);
	float content = contentRect.width;
	float scissor = scissorRect.width;

	if ((scissorX + added) >= 0.0f) {
		scissorX = 0;
		return;
	}

	if ((scissorX + added) < (-scissor + content)) {
		scissorX = (-scissor + content);
		return;
	}

	scissorX += added;
}
