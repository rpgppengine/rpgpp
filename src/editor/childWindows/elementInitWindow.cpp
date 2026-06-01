#include "childWindows/elementInitWindow.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "childWindows/popupWindow.hpp"
#include "entity.hpp"
#include "interfaceElementFactory.hpp"

ElementInitWindow::ElementInitWindow() : PopupWindow("Create Element..") {
	view = nullptr;
	currentWindow->setSize(280, 340);

	input = tgui::EditBox::create();
	input->setDefaultText("Element Name here!");
	input->setSize({"100%", 32});

	list = tgui::TreeView::create();
	list->setPosition({0, 32});
	list->setSize({"100%", "100% - 32"});

	for (auto &elementName : getElementsFactoryList()) {
		list->addItem({elementName});
	}

	std::weak_ptr<tgui::EditBox> weakInput = input;

	list->onItemSelect([this, weakInput](const tgui::String &item) {
		if (auto sharedInput = weakInput.lock()) {
			if (view->getCoordinator().getEntityManager().findName(sharedInput->getText().toStdString()) <
				MAX_ENTITIES) {
				list->deselectItem();
				return;
			}

			EntityID entity =
				construct(sharedInput->getText().toStdString(), item.toStdString(), view->getCoordinator());
			if (entity < MAX_ENTITIES) {
				view->initEntityComponents(entity);
				tree->addItem({sharedInput->getText().toStdString()});
			}
			close();
		}
	});

	currentWindow->add(input);
	currentWindow->add(list);
}

void ElementInitWindow::init() {
	input->setText("");
	list->deselectItem();
}