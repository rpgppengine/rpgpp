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

	Rectangle renderRect;

	Container();
	void update();
	void draw();

	void add(std::shared_ptr<Widget> widget);

	virtual void updateRenderRect();

	void notifyChildren(edui::Gui* gui);
};

}  // namespace edui

#endif
