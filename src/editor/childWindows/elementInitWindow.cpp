#include "childWindows/elementInitWindow.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "interfaceService.hpp"

ElementInitWindow::ElementInitWindow() : PopupWindow("Create Element..") {
	bindTranslation(this->currentWindow, "dialog.init_element.title", &tgui::ChildWindow::setTitle);
	view = nullptr;
	currentWindow->setSize(280, 340);

	input = tgui::EditBox::create();
	bindTranslation(input, "dialog.init_element.elm_name", &tgui::EditBox::setDefaultText);
	input->setSize({"100%", 32});

	list = tgui::TreeView::create();
	list->setPosition({0, 32});
	list->setSize({"100%", "100% - 32"});

	for (auto &elementName : InterfaceService::getFactory().elementNames) {
		list->addItem({elementName});
	}

	std::weak_ptr<tgui::EditBox> weakInput = input;

	list->onItemSelect([this, weakInput](const tgui::String &item) {
		if (auto sharedInput = weakInput.lock()) {
			if (view->findByName(sharedInput->getText().toStdString()) < MAX_ELEMENTS) {
				list->deselectItem();
				return;
			}

			ElementIndex elementId = view->addElement(sharedInput->getText().toStdString(), item.toStdString());
			if (elementId < MAX_ELEMENTS) {
				tree->addItem({sharedInput->getText().toStdString()});
				view->getElement(elementId)->config();
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
