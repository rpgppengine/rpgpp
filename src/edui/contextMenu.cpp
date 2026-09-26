#include "edui/contextMenu.hpp"

#include <memory>
#include <string>

#include "edui/button.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/verticalContainer.hpp"
#include "raylib.h"

using namespace edui;

ContextMenu::ContextMenu() : VerticalContainer() {
	render = std::make_unique<ContextMenuRender>();
	setSize({0, 50}, {0, 0});
	deleteOnOutsideClick = true;
}

void ContextMenu::translate() {
	layout.width.offset = 0;

	for (auto &widget : widgets) {
		std::string widgetTranslationId =
			std::string(translationId.c_str()) + '.' + std::string(widget->translationId.c_str());
		std::string widgetText = getTranslation(widgetTranslationId, widget->translationId.c_str());
		widget->as<edui::Button>().setText(widgetText);

		float fontSize = Gui::instance->labelFontSize;
		float spacing = Gui::instance->fontSpacing;
		auto textSize = MeasureTextEx(Gui::instance->font, widgetText.c_str(), fontSize, spacing);
		if (layout.width.offset < (textSize.x + 4)) {
			layout.width.offset = textSize.x + 4;
		}
	}
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

	newButton->translationId = item;

	newButton->setSize({1, 0}, {0, static_cast<int>(itemHeight)});
	newButton->setText(item);
	newButton->render->padding = 2;
	newButton->render->border = 0;
	newButton->render->as<ButtonRender>().horiAlign = HorizontalAlignment::TEXT_LEFT;
	newButton->render->as<ButtonRender>().vertAlign = VerticalAlignment::TEXT_CENTER;

	int curr = idx;
	newButton->onClicked.connect([this, item, curr] {
		onItemClicked.invoke(item, curr);
		markDelete();
	});

	idx++;

	add(newButton);
	layout.height.offset += itemHeight;
}

void ContextMenu::moveToMouse() {
	Vector2 mousePos = GetMousePosition();

	layout.x.offset = mousePos.x;
	layout.x.scale = 0;

	layout.y.offset = mousePos.y;
	layout.y.scale = 0;

	if (Gui::instance->hasMenuBar) {
		layout.y.offset -= EDUI_DEFAULT_HEIGHT;
	}
}
