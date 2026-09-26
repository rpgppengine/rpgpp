#ifndef _EDUI_LANGUAGEDROPDOWN_H
#define _EDUI_LANGUAGEDROPDOWN_H

#include "edui/dropdown.hpp"
namespace edui {
struct LanguageDropdownRender : public DropdownRender {};

struct LanguageDropdown : public Dropdown {
	LanguageDropdown();

	void addNames();
};
}  // namespace edui

#endif
