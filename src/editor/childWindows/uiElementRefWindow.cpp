#include "childWindows/uiElementRefWindow.hpp"

#include <unistd.h>

#include <cstdint>

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/popupWindow.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"

UIElementRefWindow::UIElementRefWindow() : PopupWindow("Select an Element..") {
	view = nullptr;
	currentWindow->setSize(280, 340);

	panel = tgui::ScrollablePanel::create({"100%", "100% - 36"});
	currentWindow->add(panel);

	layout = tgui::GrowVerticalLayout::create();
	panel->add(layout);

	auto none = tgui::Button::create("None");
	none->setPosition({0, "100% - 32"});
	none->setSize("100%", 32);
	none->onClick([this] {
		ref->entityId = MAX_ENTITIES;
		close();

		if (field != nullptr) {
			field->value->setText("");
		}
	});
	currentWindow->add(none);
}

void UIElementRefWindow::addButton(const std::string &title, uint8_t entityId) {
	auto newButton = tgui::Button::create(title);
	newButton->setSize("100%", 24);
	newButton->onClick([this, entityId] {
		ref->entityId = entityId;
		close();

		if (field != nullptr) {
			field->value->setText(view->getCoordinator().getEntityName(entityId));
		}
	});
	layout->add(newButton);
}

void UIElementRefWindow::init() {
	if (ref == nullptr || view == nullptr) return;

	auto &ecs = view->getCoordinator();
	int count = 0;
	for (uint16_t entity : view->getEntities()) {
		std::string name = "";
		if (entity != MAX_ENTITIES) {
			name = ecs.getEntityName(entity);
		}

		addButton(name, entity);

		count++;
	}

	auto content = panel->getContentSize();
	content.y = 24 * count;
	panel->setContentSize(content);
}