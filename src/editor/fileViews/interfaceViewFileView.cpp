#include "fileViews/interfaceViewFileView.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "editor.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
#include "uiElement.hpp"
#include "views/interfaceViewView.hpp"
#include "views/worldView.hpp"

InterfaceViewFileView::InterfaceViewFileView() {
	treeView = tgui::TreeView::create();
	treeView->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});
	treeView->setSize({RIGHT_PANEL_W, "100%"});

	widgetContainer.push_back(treeView);

	view = InterfaceViewView::create();
	view->setPosition({0, 0});
	view->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(view));

	std::weak_ptr<InterfaceViewView> weakView = view;
	treeView->onItemSelect([weakView](const tgui::String &item) {
		if (auto ptr = weakView.lock()) {
			ptr->selectElement(item.toStdString());
		}
	});

	std::weak_ptr<tgui::TreeView> weakTree = treeView;
	view->onActiveElementChanged([this, weakTree](const std::string &elementName) {
		if (auto ptr = weakTree.lock()) {
			if (elementName.empty()) {
				ptr->deselectItem();
			} else {
				ptr->selectItem({elementName});
			}
		}
	});

	widgetContainer.push_back(view);
}

void InterfaceViewFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
		const auto interface = ptr->get();

		view->setInterfaceView(interface);

		treeView->removeAllItems();
		for (auto &item : interface->getElements()) {
			treeView->addItem({item.second->getName()});
		}

		addWidgets(layout);
	}
}