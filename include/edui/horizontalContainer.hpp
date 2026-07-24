#ifndef _EDUI_HORIZONTALCONTAINER_H
#define _EDUI_HORIZONTALCONTAINER_H

#include "edui/widget.hpp"
#include "edui/container.hpp"

namespace edui {
struct HorizontalContainerRender : public ContainerRender {
	int space = 0;
};

struct HorizontalContainer : public Container {
	typedef std::shared_ptr<HorizontalContainer> Ptr;

	bool overflown = false;

	HorizontalContainer();
	void draw() override;

	void add(std::shared_ptr<Widget> widget) override;

	void scrolled(float mouseWheel) override;

	void updateContentRect() override;

	Widget::Ptr clone() override {
		return std::make_shared<HorizontalContainer>(*this);
	}
};
};

#endif
