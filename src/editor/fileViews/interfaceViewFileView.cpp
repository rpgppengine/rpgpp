#include "fileViews/interfaceViewFileView.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "editor.hpp"
#include "interfaceView.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "uiElement.hpp"
#include "views/interfaceViewView.hpp"
#include "views/worldView.hpp"
#include "widgets/anyPropertyVisitor.hpp"
#include "widgets/propertiesBox.hpp"

InterfaceViewFileView::InterfaceViewFileView() {
	treeView = tgui::TreeView::create();
	treeView->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});
	treeView->setSize({RIGHT_PANEL_W, "50%"});

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

	propertiesBox = PropertiesBox::create();
	propertiesBox->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), "50%"});
	propertiesBox->setSize({RIGHT_PANEL_W, "50%"});

	widgetContainer.push_back(propertiesBox);

	std::weak_ptr<tgui::TreeView> weakTree = treeView;
	std::weak_ptr<PropertiesBox> weakProps = propertiesBox;
	view->onActiveElementChanged([this, weakTree, weakProps, weakView](const std::string &elementName) {
		if (auto ptr = weakTree.lock()) {
			auto sharedView = weakView.lock();
			if (elementName.empty()) {
				ptr->deselectItem();
			} else {
				ptr->selectItem({elementName});
				if (!sharedView) return;
				elementProps = std::make_unique<nlohmann::json>(sharedView->getActiveElement()->dumpJson());

				if (auto sharedProps = weakProps.lock()) {
					sharedProps->addPropsJson(*elementProps);
					// auto props = sharedView->getActiveElement()->getProps();
					// AnyPropertyVisitor::addAnyProps(sharedProps.get(), props);
				}
			}
		}
	});

	propertiesBox->onJsonChanged([weakView, weakProps](nlohmann::json j) {
		if (auto sharedView = weakView.lock()) {
			if (sharedView->getActiveElement() != nullptr) {
				sharedView->getActiveElement()->fromJson(j);
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