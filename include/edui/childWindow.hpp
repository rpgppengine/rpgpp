#ifndef _EDUI_WINDOW_H
#define _EDUI_WINDOW_H

#include <memory>

#include "edui/container.hpp"
#include "edui/iconButton.hpp"
#include "edui/label.hpp"
#include "raylib.h"

namespace edui {
struct ChildWindowRender : public LabelRender {
	float titlePadding = 2.0f;
};

/* A widget which has a title bar and close button. */
struct ChildWindow : public Container {
	const float EDUI_CHILDWINDOW_BARHEIGHT = 26.0f;

	std::string title = "";
	std::string shownText = "";

	std::shared_ptr<IconButton> closeButtonPtr;

	bool dragging = false;
	bool draggable = true;

	ChildWindow();
	void applyTheme(const mINI::INIStructure &struc) override;
	void translate() override;

	/* Set the title of the window. */
	void setTitle(const std::string &title);

	void update() override;
	void draw() override;

	virtual void leftMouseClicked() override;
	virtual void leftMouseReleased() override;
	Rectangle getContentRect() override;

private:
	Rectangle getBarRect();
};
}  // namespace edui

#endif
