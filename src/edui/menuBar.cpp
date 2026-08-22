#include "edui/menuBar.hpp"

#include <memory>
#include <string>
#include <vector>

#include "edui/button.hpp"
#include "edui/contextMenu.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "raylib.h"

using namespace edui;

MenuBar::MenuBar() : HorizontalContainer() {
	render = std::make_unique<MenuBarRender>();
	render->bgColor = LIGHTGRAY;
	render->border = 0;
	render->padding = 0;
}

void MenuBar::addItem(const std::string &title, std::vector<std::string> items) {
	this->items[title] = items;

	auto button = std::make_shared<edui::Button>();
	button->render->bgColor = BLANK;
	button->render->focusBgColor = GRAY;
	button->render->border = 0;

	float totalFontSize = Gui::instance->labelFontSize;
	float spacing = Gui::instance->fontSpacing;
	auto textSize = MeasureTextEx(Gui::instance->font, title.c_str(), totalFontSize, spacing);

	button->setText(title);
	button->setSize({0, static_cast<int>(textSize.x + 4)}, {1, 0});

	std::weak_ptr<edui::Button> weakButton = button;

	button->onClicked.connect([this, title, weakButton] {
		if (auto sharedButton = weakButton.lock()) {
			printf("clicked.. \n");
			deferFlag = true;
			sharedButton->deferFlag = true;

			auto context = std::make_shared<ContextMenu>();
			context->setSize({0, 0}, {0, 0});
			context->setPosition({0, static_cast<int>(sharedButton->rect.x)},
								 {0, static_cast<int>(sharedButton->rect.y + sharedButton->rect.y)});
			for (auto &item : this->items[title]) {
				context->addItem(item);
			}

			context->deferFlag = true;

			context->onItemClicked.connect(
				[this, title](const std::string &eventItem) { onItemClicked.invoke(title, eventItem); });

			gui->addTop(context);
		}
	});

	add(button);
}
