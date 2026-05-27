#include "fileViews/interfaceViewFileView.hpp"

#include <cstdio>
#include <memory>
#include <vector>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "button.hpp"
#include "component.hpp"
#include "editor.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "lua.h"
#include "lua/reflect.hpp"
#include "lua/uiTypesApi.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "rttr/type.h"
#include "rttr/variant.h"
#include "sol/forward.hpp"
#include "sol/types.hpp"
#include "textArea.hpp"
#include "uiElement.hpp"
#include "views/interfaceViewView.hpp"
#include "views/worldView.hpp"
#include "widgets/anyPropertyVisitor.hpp"
#include "widgets/propertiesBox.hpp"

bool startsWith(const std::string &key, const std::string &prefix) {
	return (key.size() >= prefix.size()) && (key.compare(0, prefix.size(), prefix) == 0);
}

InterfaceViewFileView::InterfaceViewFileView() {
	luaState.open_libraries(sol::lib::base);
	lua_ui_types_set(luaState);

	treeView = tgui::TreeView::create();
	treeView->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});
	treeView->setSize({RIGHT_PANEL_W, "50%"});

	widgetContainer.push_back(treeView);

	view = InterfaceViewView::create();
	view->setPosition({0, 0});
	view->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(view));

	propertiesBox = PropertiesBox::create();
	propertiesBox->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), "50%"});
	propertiesBox->setSize({RIGHT_PANEL_W, "50%"});

	widgetContainer.push_back(propertiesBox);

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

		visitor.view = interface;

		view->setInterfaceView(interface);

		treeView->removeAllItems();
		for (auto &entity : interface->getEntities()) {
			treeView->addItem({ecs.getEntityName(entity)});
		}

		addWidgets(layout);
	}
}