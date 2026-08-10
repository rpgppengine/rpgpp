#ifndef _EDUI_DROPDOWNLIST_H
#define _EDUI_DROPDOWNLIST_H

#include <string>
#include "edui/signal.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"

namespace edui {
struct DropdownListRender : public VerticalContainerRender {};

struct DropdownList : public VerticalContainer {
	DropdownList();

	int idx = 0;

	SignalT<int> onValueChanged;

	void addItem(const std::string &item, int scale = 1);
};
}  // namespace edui

#endif
