#ifndef _EDUI_SCROLLABLECONTAINER_H
#define _EDUI_SCROLLABLECONTAINER_H

#include "edui/container.hpp"
namespace edui{
struct ScrollableContainerRender : public ContainerRender {
	Color scrollbarColor = GRAY;
	Color focusScrollbarColor = DARKGRAY;
	Color currentScrollbarColor = GRAY;
};

struct ScrollableContainer : public Container {
	ScrollableContainer();

	Rectangle scissorRect = {0, 0, 0, 0};

	bool isScissor = true;
	float scissorX = 0;
	float scissorY = 0;

	float scrollbarHeight = 0.0f;
	Rectangle scrollAreaRect = {0, 0, 0, 0};

	float scrollMax;

	void draw() override;

	virtual void mouseEntered() override;
	virtual void mouseLeft() override;
};
}

#endif
