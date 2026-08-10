#ifndef _EDUI_DROPDOWN_H
#define _EDUI_DROPDOWN_H

#include <memory>
#include "edui/dropdownList.hpp"
#include "edui/label.hpp"
#include "edui/valuewidget.hpp"
#include "edui/widget.hpp"

namespace edui {
struct DropdownValue {
	int idx = 0;
	std::string val = "";
};

struct DropdownRender : public LabelRender {};

struct Dropdown : public Widget, public ValueWidgetT<DropdownValue> {
	static const int EDUI_DROPDOWNLIST_HEIGHT = 50;
	static const int EDUI_DROPDOWN_MAX = 20;

	std::weak_ptr<edui::DropdownList> listPtr = {};

	Dropdown();

	virtual void setValue(const DropdownValue& val) override;
	virtual DropdownValue getValue() override;

	int currentItem = 0;
	int lastItem = 0;
	std::array<std::string, EDUI_DROPDOWN_MAX> items = {};
	std::string shownText = "";
	bool opened = false;

	void draw() override;

	void leftMouseClicked() override;

	void addItem(const std::string& item);
private:
	void openDropdown();
	void closeDropdown();
};
}  // namespace edui

#endif
