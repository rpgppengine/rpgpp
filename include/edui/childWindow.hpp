#ifndef _EDUI_WINDOW_H
#define _EDUI_WINDOW_H

#include <memory>
#include "edui/container.hpp"
#include "edui/iconButton.hpp"
#include "edui/label.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct ChildWindowRender : public ContainerRender {
	HorizontalAlignment horiAlign = HorizontalAlignment::TEXT_LEFT;
	VerticalAlignment vertAlign = VerticalAlignment::TEXT_TOP;
	float fontSize = 1;
	Color textColor = BLACK;
	float spacing = 0.05f;

	float titlePadding = 2.0f;
};

struct ChildWindow : public Container {
	const float EDUI_CHILDWINDOW_BARHEIGHT = 20.0f;

	std::string title = "";
	std::string shownText = "";

	std::shared_ptr<IconButton> closeButtonPtr;

	bool dragging = false;

	ChildWindow();

	void setTitle(const std::string& title);

	void update() override;
	void draw() override;

	virtual void leftMouseClicked() override;
	virtual void leftMouseReleased() override;
	Rectangle getContentRect() override;

private:
	Rectangle getBarRect();
};
}

#endif
