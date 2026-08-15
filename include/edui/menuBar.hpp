#ifndef _EDUI_MENUBAR_H
#define _EDUI_MENUBAR_H

#include <string>
#include <unordered_map>
#include <vector>
#include "edui/horizontalContainer.hpp"
#include "edui/signal.hpp"

namespace edui {
struct MenuBarRender : public HorizontalContainerRender {};

struct MenuBar : public HorizontalContainer {
	SignalT2<std::string, std::string> onItemClicked;

	std::unordered_map<std::string, std::vector<std::string>> items = {};

	MenuBar();

	void addItem(const std::string& title, std::vector<std::string> items);
};
}

#endif
