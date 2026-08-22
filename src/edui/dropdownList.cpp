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

void DropdownList::addItem(const std::string &item, int scale) {
	auto newButton = std::make_shared<edui::Button>();

	float fontSize = Gui::instance->labelFontSize;

	newButton->setSize({1, 0}, {0, 22});
	newButton->setText(item);
	newButton->render->padding = 2;
	newButton->render->as<ButtonRender>().horiAlign = HorizontalAlignment::TEXT_LEFT;
	newButton->render->as<ButtonRender>().vertAlign = VerticalAlignment::TEXT_CENTER;

	int curr = idx;
	newButton->onClicked.connect([this, curr] {
		onValueChanged.invoke(curr);
		markDelete();
	});

	idx++;

	add(newButton);
}
