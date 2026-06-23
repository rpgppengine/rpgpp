#include "fileViews/interfaceViewFileView.hpp"

#include <cstdio>
#include <memory>
#include <vector>

#include "TGUI/String.hpp"
#include "TGUI/ToolTip.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/TabContainer.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "childWindows/elementInitWindow.hpp"
#include "component.hpp"
#include "editor.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "interfaceView.hpp"
#include "lua.h"
#include "lua/reflect.hpp"
#include "lua/uiTypesApi.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "rttr/type.h"
#include "rttr/variant.h"
#include "services/fileSystemService.hpp"
#include "sol/forward.hpp"
#include "sol/types.hpp"
#include "views/interfaceViewView.hpp"
#include "views/worldView.hpp"
#include "widgets/anyPropertyVisitor.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/fileField.hpp"

bool startsWith(const std::string &key, const std::string &prefix) {
	return (key.size() >= prefix.size()) && (key.compare(0, prefix.size(), prefix) == 0);
}

InterfaceViewFileView::InterfaceViewFileView() {
	auto createButton = tgui::Button::create("Create..");
	createButton->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});
	createButton->setSize({RIGHT_PANEL_W, 32});
	createButton->onClick([this] {
		auto *ptr = Editor::instance->getGui().getChildWindowSubService()->getWindow("create_ui_element");
		ElementInitWindow *window = static_cast<ElementInitWindow *>(ptr);
		window->view = view->ptr;
		window->tree = treeView.get();
		window->init();
		window->open();
	});
	widgetContainer.push_back(createButton);

	treeView = tgui::TreeView::create();
	treeView->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 32});
	treeView->setSize({RIGHT_PANEL_W, "50% - 32"});

	widgetContainer.push_back(treeView);

	view = InterfaceViewView::create();
	view->setPosition({0, 0});
	view->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(view));

	auto tabsContainer = tgui::TabContainer::create();
	tabsContainer->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), "50%"});
	tabsContainer->setSize({RIGHT_PANEL_W, "50%"});

	auto filePropertiesPanel = tabsContainer->addTab("UI View");
	auto elementPropertiesPanel = tabsContainer->addTab("Element");

	widgetContainer.push_back(tabsContainer);

	// view properties
	scriptFileField = FileField::create();
	scriptFileField->setSize({"100%", 24});
	scriptFileField->label->setText("Script");
	scriptFileField->pathFilters = {{"Script", {"*.lua"}}};
	scriptFileField->callback = [this](const tgui::String &path) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(this->variant);
		const auto interface = ptr->get();

		std::string fileName = GetFileName(path.toStdString().c_str());
		interface->setScriptFile(fileName);
	};

	filePropertiesPanel->add(scriptFileField);

	// element properties
	propertiesBox = PropertiesBox::create();
	propertiesBox->setSize("100%", "100%");

	elementPropertiesPanel->add(propertiesBox);

	std::weak_ptr<tgui::TreeView> weakTree = treeView;
	std::weak_ptr<PropertiesBox> weakProps = propertiesBox;

	std::weak_ptr<InterfaceViewView> weakView = view;
	treeView->onItemSelect([weakView, this, weakProps](const tgui::String &item) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(this->variant);
		const auto interface = ptr->get();
		auto &ecs = interface->getCoordinator();

		EntityID entity = ecs.getEntityManager().findName(item.toStdString());

		if (entity >= MAX_ENTITIES) return;

		if (auto ptr = weakView.lock()) {
			ptr->selectElement(item.toStdString());

			if (auto sharedProps = weakProps.lock()) {
				sharedProps->clear();

				auto set = ecs.getEntityComponents(entity);
				for (auto &name : set) {
					auto componentVariant = ecs.getComponentVariant(entity, name);
					visitor.componentVisit(componentVariant, sharedProps.get());
				}
			}
		}
	});

	view->onActiveEntityChanged([weakTree, weakView, this, weakProps](EntityID entity) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(this->variant);
		const auto interface = ptr->get();
		auto &ecs = interface->getCoordinator();

		if (auto ptr = weakTree.lock()) {
			auto sharedView = weakView.lock();
			if (entity >= MAX_ENTITIES) {
				ptr->deselectItem();
				if (auto sharedProps = weakProps.lock()) {
					sharedProps->clear();
				}
			} else {
				ptr->selectItem({visitor.view->getCoordinator().getEntityName(entity)});
				if (!sharedView) return;

				if (auto sharedProps = weakProps.lock()) {
					sharedProps->clear();

					auto set = ecs.getEntityComponents(entity);
					for (auto &name : set) {
						auto componentVariant = ecs.getComponentVariant(entity, name);
						visitor.componentVisit(componentVariant, sharedProps.get());
					}
				}
			}
		}
	});

	treeView->onRightClick([this](const tgui::String &item) {
		selectedElement = item.toStdString();
		elementContextMenu->setPosition(GetMousePosition().x, GetMousePosition().y);
		elementContextMenu->openMenu();
	});

	elementContextMenu = tgui::ContextMenu::create();
	elementContextMenu->addMenuItem("Delete");
	elementContextMenu->onMenuItemClick([this, weakTree](const std::vector<tgui::String> &hierarchy) {
		if (hierarchy.empty()) return;

		auto item = hierarchy[0];

		if (item == "Delete") {
			const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
			const auto interface = ptr->get();

			auto &ecs = interface->getCoordinator();

			EntityID entity = ecs.getEntityManager().findName(selectedElement);

			if (entity < MAX_ENTITIES) {
				ecs.destroyEntity(entity);
				if (auto sharedTree = weakTree.lock()) {
					sharedTree->removeItem({selectedElement});
				}
			}
		}
	});
	Editor::instance->getGui().gui->add(elementContextMenu);

	view->propBox = propertiesBox.get();
	view->visitor = &visitor;

	widgetContainer.push_back(view);
}

void InterfaceViewFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
		const auto interface = ptr->get();

		auto &ecs = interface->getCoordinator();

		for (auto &entity : interface->getEntities()) {
			interface->initEntityComponents(entity);
		}

		visitor.view = interface;

		view->setInterfaceView(interface);

		treeView->removeAllItems();
		for (auto &entity : interface->getEntities()) {
			treeView->addItem({ecs.getEntityName(entity)});
		}

		scriptFileField->setValue(interface->getScriptFile());

		addWidgets(layout);
	}
}