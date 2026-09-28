#include "edui/menuBar.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/contextMenu.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

MenuBar::MenuBar() : HorizontalContainer() {
	render = std::make_unique<MenuBarRender>();
	render->bgColor = LIGHTGRAY;
	render->border = 0;
	render->padding = 0;
}

void MenuBar::applyTheme(const mINI::INIStructure &struc) {
	Container::applyTheme(struc);
	INISection section = struc.get("Widget");
	for (auto &widget : widgets) {
		widget->render->bgColor = parseColorString(section.get("SecondaryColor"));
		widget->render->focusBgColor = parseColorString(section.get("TertiaryColor"));
		widget->unfocused();
	}
}

void MenuBar::translate() {
	for (auto &widget : widgets) {
		auto titleTranslationId = std::string(translationId.c_str()) + '.' + std::string(widget->translationId.c_str());
		std::string titleText = getTranslation(titleTranslationId, widget->translationId.c_str());
		widget->as<edui::Button>().setText(titleText);
		widget->as<edui::Button>().setWidthFit();
	}
}

void MenuBar::draw() {
	auto &rend = render->as<ContainerRender>();

	DrawRectangleRec(rect, rend.secondaryColor);

	drawChildren();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void MenuBar::addItem(const std::string &title, std::vector<std::string> items) {
	this->items[title] = items;
	size++;
	int curr = size - 1;

	auto button = std::make_shared<edui::Button>();
	button->translationId = title;
	button->render->bgColor = BLANK;
	button->render->focusBgColor = GRAY;
	button->render->border = 0;

	float totalFontSize = Gui::instance->labelFontSize;
	float spacing = Gui::instance->fontSpacing;
	auto textSize = MeasureTextEx(Gui::instance->font, title.c_str(), totalFontSize, spacing);

	// button->setText(title);
	auto titleTranslationId = std::string(translationId.c_str()) + '.' + title;
	button->setText(getTranslation(titleTranslationId, title));
	button->setSize({0, static_cast<int>(textSize.x + 4)}, {1, 0});

	std::weak_ptr<edui::Button> weakButton = button;

	button->onClicked.connect([this, curr, title, weakButton, titleTranslationId] {
		if (auto sharedButton = weakButton.lock()) {
			if (currentContextMenu != nullptr) {
				currentContextMenu->markDelete();
			}

			auto context = std::make_shared<ContextMenu>();
			context->outsideClickException = widgets[curr]->rect;
			context->setSize({0, 0}, {0, 0});
			context->setPosition({0, static_cast<int>(sharedButton->rect.x)},
								 {0, static_cast<int>(sharedButton->rect.y + sharedButton->rect.y)});
			for (auto &item : this->items[title]) {
				auto itemTranslationId = std::string(titleTranslationId) + '.' + item;
				context->addItem(getTranslation(itemTranslationId, item));
			}

			context->onItemClicked.connect([this, title](const std::string &eventItem, size_t index) {
				onItemClicked.invoke(title, eventItem);
				onItemClickedInt.invoke(size - 1, index);
			});

			this->currentContextMenu = context;
			gui->add(context, EDUI_MAX_LAYERS - 1);
		}
	});

	add(button);
}
