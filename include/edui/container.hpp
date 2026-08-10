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

struct ContainerRender : public WidgetRender {
	Color scrollbarColor = GRAY;
	Color focusScrollbarColor = DARKGRAY;
	Color currentScrollbarColor = GRAY;
};

struct Container : public Widget {
	Gui* gui;

	typedef std::shared_ptr<Container> Ptr;

	std::vector<std::shared_ptr<Widget>> widgets = {};

	Rectangle contentRect = {0, 0, 0, 0};
	Rectangle scissorRect = {0, 0, 0, 0};
	Rectangle renderRect;

	bool isScissor = true;
	float scissorX = 0;
	float scissorY = 0;

	float scrollbarHeight = 0.0f;
	Rectangle scrollAreaRect = {0, 0, 0, 0};

	float scrollMax;

	Container();
	virtual void update();
	virtual void draw();

	virtual void mouseEntered();
	virtual void mouseLeft();

	virtual void add(std::shared_ptr<Widget> widget);

	virtual void updateContentRect();

	void notifyChildren(edui::Gui *gui);

	Widget::Ptr clone() { return std::make_shared<Container>(*this); }

	void markDelete();
};

}  // namespace edui

#endif
