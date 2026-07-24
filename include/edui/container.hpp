#ifndef _EDUI_CONTAINER_H
#define _EDUI_CONTAINER_H

#include <cstdint>
#include <vector>

#include "edui/gui.hpp"
#include "edui/widget.hpp"
namespace edui {
const uint8_t ScrollSpeed = 36;
const uint8_t ScrollbarSize = 8;

struct ContainerRender : public WidgetRender {};

struct Container : public Widget {
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
	void update();
	virtual void draw();

	virtual void add(std::shared_ptr<Widget> widget);

	virtual void updateContentRect();

	void notifyChildren(edui::Gui* gui);

	Widget::Ptr clone() {
		return std::make_shared<Container>(*this);
	}
};

}  // namespace edui

#endif
