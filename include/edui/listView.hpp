#ifndef _EDUI_LISTVIEW_H
#define _EDUI_LISTVIEW_H

#include <memory>

#include "edui/iconTextButton.hpp"
#include "edui/verticalContainer.hpp"
namespace edui {
struct ListViewRender : public VerticalContainerRender {};

struct ListView : public VerticalContainer {
	const float SCROLL_DEBOUNCE = 5;

	ListView();
	void update() override;
	void draw() override;

	void leftMouseReleased() override;

	void addItem(const std::string &title, int iconId);

private:
	bool holdingMouse = false;
	bool notifiedMouseRelease = true;
	int widgetRefer = -1;
	int heldIndex = -1;
	std::weak_ptr<edui::IconTextButton> heldItem;
	Vector2 mouseOffset = {0, 0};
	float scrollDebounce = 0;
};
}  // namespace edui

#endif
