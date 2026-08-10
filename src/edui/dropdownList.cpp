#include "edui/dropdownList.hpp"
#include <memory>
#include "edui/button.hpp"
#include "edui/gui.hpp"
#include "edui/label.hpp"
#include "edui/verticalContainer.hpp"

using namespace edui;

DropdownList::DropdownList() : VerticalContainer() {
	render = std::make_unique<DropdownListRender>();
	deleteOnOutsideClick = true;
}

void DropdownList::addItem(const std::string& item, int scale) {
	auto newButton = std::make_shared<edui::Button>();

	auto fontSize = Gui::instance->font.baseSize * scale;

	newButton->setSize({1, 0}, {0, fontSize - 4});
	newButton->setText(item);
	newButton->render->padding = 2;
	newButton->render->as<ButtonRender>().fontSize = scale;
	newButton->render->as<ButtonRender>().horiAlign = HorizontalAlignment::TEXT_LEFT;

	int curr = idx;
	newButton->clicked = [this, curr] {
		onValueChanged.invoke(curr);
		markDelete();
	};

	idx++;

	add(newButton);
}
