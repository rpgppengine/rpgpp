#include "edui/languageDropdown.hpp"

#include <memory>

#include "edui/dropdown.hpp"
#include "edui/gui.hpp"

using namespace edui;

LanguageDropdown::LanguageDropdown() : Dropdown() {
	translated = false;

	render = std::make_unique<edui::LanguageDropdownRender>();
	render->padding = 2;
	render->as<DropdownRender>().horiAlign = HorizontalAlignment::TEXT_LEFT;
	render->as<DropdownRender>().vertAlign = VerticalAlignment::TEXT_CENTER;

	addNames();
}

void LanguageDropdown::addNames() {
	for (int i = 0; i < Gui::instance->languageNamesCount; i++) {
		auto name = std::string(Gui::instance->languageNames[i].value.c_str());
		addItem(name);
	}
}
