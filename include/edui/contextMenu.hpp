#ifndef _EDUI_CONTEXTMENU_H
#define _EDUI_CONTEXTMENU_H

#include <string>
#include "edui/label.hpp"
#include "edui/signal.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
namespace edui {
struct ContextMenuRender : public VerticalContainerRender {};

struct ContextMenu : public VerticalContainer {
	SignalT<std::string> onItemClicked;

	ContextMenu();

	int idx = 0;
	void addItem(const std::string& item);
};
}

#endif
