#ifndef _EDUI_CONTAINER_H
#define _EDUI_CONTAINER_H

#include <cstdint>
#include <vector>

#include "edui/gui.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
const uint8_t ScrollSpeed = 36;
const uint8_t ScrollbarSize = 12;

struct ContainerRender : public WidgetRender {};

struct Container : public Widget {
	Gui *gui;

	typedef std::shared_ptr<Container> Ptr;

	std::vector<std::shared_ptr<Widget>> widgets = {};

	Rectangle contentRect = {0, 0, 0, 0};
	Rectangle renderRect;

	Container();
	virtual void update();
	virtual void draw();

	virtual void add(std::shared_ptr<Widget> widget);

	virtual void updateContentRect();

	void notifyChildren(edui::Gui *gui);

	Widget::Ptr clone() { return std::make_shared<Container>(*this); }

	void markDelete();
};

}  // namespace edui

#endif
