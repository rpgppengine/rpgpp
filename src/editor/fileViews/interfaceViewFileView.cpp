#include "fileViews/interfaceViewFileView.hpp"

#include <cstdio>
#include <memory>
#include <vector>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "button.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
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
	lua_ui_types_set(luaState);

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
					// sharedProps->addPropsJson(*elementProps);
					//  auto props = sharedView->getActiveElement()->getProps();
					//  AnyPropertyVisitor::addAnyProps(sharedProps.get(), props);
					auto elementLua = luaState[elementName];
					if (!elementLua.valid()) return;

					printf("%s: \n", elementName.c_str());

					sol::table meta = elementLua[sol::metatable_key];
					for (auto &j : meta) {
						const std::string key = j.first.as<std::string>();
						sol::object val = j.second;

						std::vector<sol::object> props;

						if (!startsWith(key, "__") && !startsWith(key, "class_")) {
							if (key != "new") {
								printf("%s \n", key.c_str());
							}
						}
					}
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

		for (auto &item : interface->getElements()) {
			/*
			auto type = item.second->getType();
			switch (type) {
				case INTERFACE_TEXTAREA:
					luaState[item.second->getName()] = static_cast<TextArea *>(item.second.get());
					break;
				case INTERFACE_BUTTON:
					luaState[item.second->getName()] = static_cast<Button *>(item.second.get());
					break;
				default:
					luaState[item.second->getName()] = item.second.get();
					break;
			}
					*/
			// auto variant = rttr::variant(item.second);
			printf("%s \n",
				   rttr::type::get(item.second).get_wrapped_type().get_raw_type().get_name().to_string().c_str());
			for (auto &prop : rttr::type::get(item.second).get_wrapped_type().get_raw_type().get_properties()) {
				printf("%s \n", prop.get_name().to_string().c_str());
			}
		}

		view->setInterfaceView(interface);

		treeView->removeAllItems();
		for (auto &item : interface->getElements()) {
			treeView->addItem({item.second->getName()});
		}

		addWidgets(layout);
	}
}