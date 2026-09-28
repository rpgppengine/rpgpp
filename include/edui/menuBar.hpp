#ifndef _EDUI_MENUBAR_H
#define _EDUI_MENUBAR_H

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "edui/contextMenu.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/signal.hpp"

namespace edui {
struct MenuBarRender : public HorizontalContainerRender {};

struct MenuBar : public HorizontalContainer {
	SignalT2<std::string, std::string> onItemClicked;
	SignalT2<size_t, size_t> onItemClickedInt;

	std::unordered_map<std::string, std::vector<std::string>> items = {};
	size_t size = 0;
	std::shared_ptr<edui::ContextMenu> currentContextMenu = nullptr;

	MenuBar();
	void applyTheme(const mINI::INIStructure &struc);
	void translate();

	void draw();

	void addItem(const std::string &title, std::vector<std::string> items);
};
}  // namespace edui

#endif
