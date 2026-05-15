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

		auto &ecs = interface->getCoordinator();

		// register in lua
		for (auto &entity : interface->getEntities()) {
			sol::table tbl = luaState.create_named_table(TextFormat("Entity_%i", entity));

			auto set = ecs.getEntityComponents(entity);
			for (auto &name : set) {
				tbl[name] = ecs.getLuaObject(entity, name, luaState.lua_state());
			}
		}

		// test with script string
		std::string testCode = R"(
			print('hello, let me test Entity_0.Rectangle')
			print(Entity_0.Rectangle.x)
			print(Entity_0.Rectangle.y)
			print(Entity_0.Rectangle.width)
			print(Entity_0.Rectangle.height)

			print('the entity has a gray color')
			print(Entity_0.ColorRectComponent.color.r)
			print(Entity_0.ColorRectComponent.color.g)
			print(Entity_0.ColorRectComponent.color.b)
			print(Entity_0.ColorRectComponent.color.a)
		)";
		luaState.script(testCode);

		// add to propertiesBox
		for (auto &entity : interface->getEntities()) {
			auto set = ecs.getEntityComponents(entity);
			for (auto &name : set) {
				auto variant = ecs.getComponentVariant(entity, name);
				visitor.visit(name, variant, propertiesBox.get());
			}
		}

		// testing lua
		for (auto &entity : interface->getEntities()) {
			printf("Entity #%i: \n", entity);
			sol::table tbl = luaState[TextFormat("Entity_%i", entity)];

			for (auto &item : tbl) {
				if (item.first.is<std::string>()) {
					printf("%s: %i, %i\n", item.first.as<std::string>().c_str(), item.second.get_type(),
						   item.second.valid());
				}

				if (item.second.valid()) {
					sol::table meta = item.second.as<sol::userdata>()[sol::metatable_key];
					for (auto &j : meta) {
						auto key = j.first.as<std::string>();
						auto value = j.second;
						if (!startsWith(key, "__") && !startsWith(key, "class_")) {
							if (key != "new") {
								// print the property
								printf("%s: %i \n", key.c_str(), value.get_type());
								if (value.is<float>()) {
									printf("%f \n", value.as<float>());
								}
								/*
								if (value.get_type() == sol::type::function) {
									printf("function \n");
									sol::function f = value;
									auto res = f();
									if (res.valid()) {
										for (auto r : res) {
											printf("%i", r.get_type());
										}
									} else {
										printf("not valid.. \n");
									}
								}
									*/
							}
						}
					}
				}
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