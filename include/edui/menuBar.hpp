#ifndef _EDUI_MENUBAR_H
#define _EDUI_MENUBAR_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "edui/horizontalContainer.hpp"
#include "edui/signal.hpp"

namespace edui {
struct MenuBarRender : public HorizontalContainerRender {};

struct MenuBar : public HorizontalContainer {
	SignalT2<std::string, std::string> onItemClicked;
	SignalT2<size_t, size_t> onItemClickedInt;

	std::unordered_map<std::string, std::vector<std::string>> items = {};
	size_t size = 0;

	MenuBar();
	void translate();

	void addItem(const std::string &title, std::vector<std::string> items);
};
}  // namespace edui

#endif
