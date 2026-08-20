#ifndef _EDUI_HORIZONTALCONTAINER_H
#define _EDUI_HORIZONTALCONTAINER_H

#include "edui/container.hpp"
#include "edui/scrollableContainer.hpp"
#include "edui/widget.hpp"

namespace edui {
struct HorizontalContainerRender : public ScrollableContainerRender {
	int space = 0;
};

struct HorizontalContainer : public ScrollableContainer {
	typedef std::shared_ptr<HorizontalContainer> Ptr;

	bool overflown = false;
	Rectangle scrollbarRect = {0, 0, 0, 0};
	bool scrolling = false;
	Vector2 scrollOffset = {0, 0};

	bool reverse = false;

	HorizontalContainer();
	void update() override;
	void draw() override;

	void add(std::shared_ptr<Widget> widget) override;

	void scrolled(float mouseWheel) override;
	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void updateContentRect() override;

	Widget::Ptr clone() override { return std::make_shared<HorizontalContainer>(*this); }
};
};	// namespace edui

#endif
