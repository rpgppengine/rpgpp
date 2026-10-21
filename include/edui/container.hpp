#ifndef _EDUI_CONTAINER_H
#define _EDUI_CONTAINER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "edui/gui.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
const uint8_t ScrollSpeed = 36;
const uint8_t ScrollbarSize = 12;

struct ContainerRender : public WidgetRender {};

/* Widget, which contains children widgets. */
struct Container : public Widget {
	Gui *gui;

	typedef std::shared_ptr<Container> Ptr;

	std::vector<std::shared_ptr<Widget>> widgets = {};

	Rectangle contentRect = {0, 0, 0, 0};
	Rectangle renderRect;

	bool mouseIsInContent() { return CheckCollisionPointRec(GetMousePosition(), contentRect); }

	Container();
	void applyTheme(const mINI::INIStructure &struc) override;
	void translate() override;

	virtual void update() override;
	virtual void draw() override;

	/* Add a child widget. */
	virtual void add(std::shared_ptr<Widget> widget);

	virtual void updateContentRect();

	void notifyChildren(edui::Gui *gui);

	Widget::Ptr clone() override { return std::make_shared<Container>(*this); }

	/* Get a child, identified by referId. */
	std::shared_ptr<Widget> getWidgetById(int referId);
	void markDelete() override;
	void deleteChildren();

protected:
	void drawChildren();
};

}  // namespace edui

#endif
