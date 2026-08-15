#include "edui/contextMenu.hpp"

#include <memory>

#include "edui/button.hpp"
#include "edui/verticalContainer.hpp"
#include "raylib.h"

using namespace edui;

ContextMenu::ContextMenu() : VerticalContainer() {
	render = std::make_unique<ContextMenuRender>();
	deleteOnOutsideClick = true;
}

void ContextMenu::addItem(const std::string &item) {
	float itemHeight = 22.0f;

	auto newButton = std::make_shared<edui::Button>();

	float fontSize = Gui::instance->labelFontSize;
	float spacing = Gui::instance->fontSpacing;
	auto textSize = MeasureTextEx(Gui::instance->font, item.c_str(), fontSize, spacing);

	if (layout.width.offset < (textSize.x + 4)) {
		layout.width.offset = textSize.x + 4;
	}

	newButton->setSize({1, 0}, {0, static_cast<int>(itemHeight)});
	newButton->setText(item);
	newButton->render->padding = 2;
	newButton->render->border = 0;
	newButton->render->as<ButtonRender>().horiAlign = HorizontalAlignment::TEXT_LEFT;
	newButton->render->as<ButtonRender>().vertAlign = VerticalAlignment::TEXT_CENTER;

	int curr = idx;
	newButton->clicked = [this, item] {
		onItemClicked.invoke(item);
		markDelete();
	};

	idx++;

	add(newButton);
	layout.height.offset += itemHeight;
}
