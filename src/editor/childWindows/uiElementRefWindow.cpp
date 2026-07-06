#include "childWindows/uiElementRefWindow.hpp"

#include <cstdint>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "childWindows/popupWindow.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"

UIElementRefWindow::UIElementRefWindow() : PopupWindow("Select an Element..") {
	view = nullptr;
	currentWindow->setSize(280, 340);

	auto none = tgui::Button::create("None");
	none->setPosition({0, "100% - 32"});
	none->setSize("100%", 32);
	none->onClick([this] {
		ref->entityId = MAX_ELEMENTS;
		close();

		if (field != nullptr) {
			field->value->setText("");
		}
	});
	currentWindow->add(none);

	list = tgui::TreeView::create();
	list->setSize({"100%", "100% - 32"});
	list->onItemSelect([this](const tgui::String &item) {
		auto entity = view->findByName(item.toStdString());
		ref->entityId = entity;
		close();

		if (field != nullptr && entity < MAX_ELEMENTS) {
			field->value->setText(item.toStdString());
		}
	});
	currentWindow->add(list);
}

void UIElementRefWindow::init() {
	if (ref == nullptr || view == nullptr) return;

	list->removeAllItems();

	ElementIndex i = 0;
	for (auto&& entity : view->getElements()) {
		if (entity.get() != nullptr) {
			std::string name = view->getEntityName(i);

			list->addItem({name});
		}

		i++;
	}
}
