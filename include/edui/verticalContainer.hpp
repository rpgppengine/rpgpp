#ifndef _EDUI_VERTICALCONTAINER_H
#define _EDUI_VERTICALCONTAINER_H

#include <memory>
#include "edui/widget.hpp"
#include "edui/container.hpp"

namespace edui {
struct VerticalContainerRender : public ContainerRender {
	int space = 0;
};

struct VerticalContainer : public Container {
	typedef std::shared_ptr<VerticalContainer> Ptr;

	bool overflown = false;

	VerticalContainer();
	void draw() override;

	void add(std::shared_ptr<Widget> widget) override;

	void scrolled(float mouseWheel) override;

	void updateContentRect() override;

	Widget::Ptr clone() override {
		return std::make_shared<VerticalContainer>(*this);
	}
};
};

#endif
