#ifndef _EDUI_LANGUAGEDROPDOWN_H
#define _EDUI_LANGUAGEDROPDOWN_H

#include "edui/dropdown.hpp"
namespace edui {
struct LanguageDropdownRender : public DropdownRender {};

/* A specialized dropdown which includes the names of the languages that are loaded. */
struct LanguageDropdown : public Dropdown {
	LanguageDropdown();

	void addNames();
};
}  // namespace edui

#endif
