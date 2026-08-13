#ifndef _EDUI_VERTICALCONTAINER_H
#define _EDUI_VERTICALCONTAINER_H

#include <memory>

#include "edui/container.hpp"
#include "edui/scrollableContainer.hpp"
#include "edui/widget.hpp"

namespace edui {
struct VerticalContainerRender : public ScrollableContainerRender {
	int space = 0;
};

struct VerticalContainer : public ScrollableContainer {
	typedef std::shared_ptr<VerticalContainer> Ptr;

	bool overflown = false;
	Rectangle scrollbarRect = {0, 0, 0, 0};
	bool scrolling = false;
	Vector2 scrollOffset = {0, 0};

	VerticalContainer();
	void update() override;
	void draw() override;

	void add(std::shared_ptr<Widget> widget) override;

	void scrolled(float mouseWheel) override;
	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void updateContentRect() override;

	Widget::Ptr clone() override { return std::make_shared<VerticalContainer>(*this); }
};
};	// namespace edui

#endif
