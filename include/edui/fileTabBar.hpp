#ifndef _EDUI_FILETABBAR_H
#define _EDUI_FILETABBAR_H

#include "edui/tabBar.hpp"
namespace edui {
struct FileTabBarRender : public TabBarRender {};

struct FileTabBar : public TabBar {
	FileTabBar();

	std::shared_ptr<Container> addItem(const std::string &item, int iconId) override;
};
}

#endif
