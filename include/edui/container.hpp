#ifndef _EDUI_CONTAINER_H
#define _EDUI_CONTAINER_H

#include <vector>

#include "edui/gui.hpp"
#include "edui/widget.hpp"
namespace edui {
struct ContainerRender : public WidgetRender {};

struct Container : public Widget {
	typedef std::shared_ptr<Container> Ptr;

	std::vector<std::shared_ptr<Widget>> widgets = {};

	Rectangle contentRect;
	Rectangle scissorRect = {0, 0, 0, 0};
	Rectangle renderRect;

	bool isScissor = true;
	float scissorX = 0;
	float scissorY = 0;

	Container();
	void update();
	void draw();

	virtual void add(std::shared_ptr<Widget> widget);

	virtual void updateContentRect();

	void notifyChildren(edui::Gui* gui);

	Widget::Ptr clone() {
		return std::make_shared<Container>(*this);
	}
};

}  // namespace edui

#endif
