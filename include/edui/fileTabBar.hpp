#ifndef _EDUI_FILETABBAR_H
#define _EDUI_FILETABBAR_H

#include "edui/tabBar.hpp"

namespace edui {
struct FileTabBarRender : public TabBarRender {};

struct FileTabBar : public TabBar {
	FileTabBar();
	void update() override;

	std::shared_ptr<Container> addItem(const std::string &item, int iconId) override;
	void showActiveTab(int index);
};
}  // namespace edui

#endif
