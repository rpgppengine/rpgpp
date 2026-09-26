#ifndef _EDUI_DROPDOWNLIST_H
#define _EDUI_DROPDOWNLIST_H

#include <string>

#include "edui/signal.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"

namespace edui {
struct DropdownListRender : public VerticalContainerRender {};

struct DropdownList : public VerticalContainer {
	SignalT<int> onValueChanged;

	int idx = 0;
	bool translated = true;

	DropdownList();
	void translate() override;

	void addItem(const std::string &item, int scale = 1);
};
}  // namespace edui

#endif
