#include "edui/scrollableContainer.hpp"

#include <memory>

#include "edui/container.hpp"

using namespace edui;

ScrollableContainer::ScrollableContainer() : Container() { render = std::make_unique<ScrollableContainerRender>(); }

void ScrollableContainer::draw() {
	auto &rend = render->as<ScrollableContainerRender>();

	DrawRectangleRec(rect, rend.bgColor);

	if (isScissor) {
		BeginScissorMode(contentRect.x, contentRect.y, contentRect.width, contentRect.height);
	}

	drawChildren();

	if (isScissor) EndScissorMode();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}
