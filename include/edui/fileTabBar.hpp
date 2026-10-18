#ifndef _EDUI_FILETABBAR_H
#define _EDUI_FILETABBAR_H

#include "edui/fileTab.hpp"
#include "edui/tabBar.hpp"

namespace edui {
struct FileTabBarRender : public TabBarRender {};

struct FileTabBar : public TabBar {
	const float SCROLL_DEBOUNCE = 5;

	FileTabBar();
	void update() override;
	void draw() override;

	std::shared_ptr<Container> addItem(const std::string &item, int iconId) override;
	void showActiveTab(int index);

private:
	bool holdingMouse = false;
	bool notifiedMouseRelease = true;
	int widgetRefer = -1;
	int heldIndex = -1;
	std::weak_ptr<edui::FileTab> heldItem;
	Vector2 mouseOffset = {0, 0};
	float scrollDebounce = 0;

	void onFileTabBarMouseRelease();
};
}  // namespace edui

#endif
