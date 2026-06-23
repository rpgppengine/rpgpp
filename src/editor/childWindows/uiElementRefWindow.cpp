#include "childWindows/uiElementRefWindow.hpp"

#include <cstdint>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "childWindows/popupWindow.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"

UIElementRefWindow::UIElementRefWindow() : PopupWindow("Select an Element..") {
	view = nullptr;
	currentWindow->setSize(280, 340);

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

	list = tgui::TreeView::create();
	list->setSize({"100%", "100% - 32"});
	list->onItemSelect([this](const tgui::String &item) {
		auto entity = view->getCoordinator().getEntityManager().findName(item.toStdString());
		ref->entityId = entity;
		close();

		if (field != nullptr && entity < MAX_ENTITIES) {
			field->value->setText(item.toStdString());
		}
	});
	currentWindow->add(list);
}

void UIElementRefWindow::init() {
	if (ref == nullptr || view == nullptr) return;

	list->removeAllItems();

	auto &ecs = view->getCoordinator();
	int count = 0;
	for (uint16_t entity : view->getEntities()) {
		std::string name = "";
		if (entity != MAX_ENTITIES) {
			name = ecs.getEntityName(entity);
		}

		list->addItem({name});

		count++;
	}
}
