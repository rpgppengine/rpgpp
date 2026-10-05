#ifndef _EDUI_HORIZONTALCONTAINER_H
#define _EDUI_HORIZONTALCONTAINER_H

#include <memory>

#include "edui/container.hpp"
#include "edui/horizontalScrollbar.hpp"
#include "edui/scrollableContainer.hpp"
#include "edui/widget.hpp"

namespace edui {
struct HorizontalContainerRender : public ScrollableContainerRender {
	int space = 0;
};

struct HorizontalContainer : public ScrollableContainer {
	typedef std::shared_ptr<HorizontalContainer> Ptr;

	std::shared_ptr<HorizontalScrollbar> scrollbar;

	bool overflown = false;
	Rectangle scrollbarRect = {0, 0, 0, 0};
	bool scrolling = false;
	Vector2 scrollOffset = {0, 0};

	bool reverse = false;

	HorizontalContainer();
	void applyTheme(const mINI::INIStructure &struc) override;

	void update() override;
	void draw() override;

	void add(std::shared_ptr<Widget> widget) override;

	void scrolled(float mouseWheel) override;
	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void updateContentRect() override;

	Widget::Ptr clone() override { return std::make_shared<HorizontalContainer>(*this); }

	void updateWidgets();
	void fixScroll();
};
};	// namespace edui

#endif
