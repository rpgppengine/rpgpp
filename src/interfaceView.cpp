#include "interfaceView.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "actor.hpp"
#include "component.hpp"
#include "coordinator.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "lua.h"
#include "raylib.h"
#include "sol/state_view.hpp"
#include "system.hpp"

InterfaceView::InterfaceView() : InterfaceView(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;

	ecs.init();

	ecs.registerComponent<VisibilityComponent>();
	ecs.registerComponent<Rectangle>();
	ecs.registerComponent<LabelComponent>();
	ecs.registerComponent<TextAreaComponent>();
	ecs.registerComponent<ColorRectComponent>();
	ecs.registerComponent<ImageRectComponent>();
	ecs.registerComponent<NinePatchImageRectComponent>();
	ecs.registerComponent<DialogueComponent>();
	ecs.registerComponent<ButtonComponent>();
	ecs.registerComponent<InputComponent>();
}

InterfaceView::InterfaceView(const std::string &filePath) : InterfaceView(Rectangle{}) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};

	for (auto &item : j.at("entities").items()) {
		auto obj = item.value();
		auto entity = ecs.createEntity(item.key());
		for (auto &componentJson : item.value().items()) {
			ecs.insertComponentFromJson(entity, componentJson.key(), componentJson.value());
		}
	}
}

InterfaceView::InterfaceView(InterfaceViewBin &bin) : InterfaceView(Rectangle{}) {
	for (auto &[title, elementCbor] : bin.entites) {
		auto entity = ecs.createEntity(title);

		auto elementJson = json::from_cbor(elementCbor);
		for (auto &componentJson : elementJson.items()) {
			ecs.insertComponentFromJson(entity, componentJson.key(), componentJson.value());
		}
		initEntityComponents(entity);
	}
}

nlohmann::json InterfaceView::dumpJson() {
	auto j = json::object();

	j["entities"] = json::object();
	for (auto &entity : ecs.getEntities()) {
		auto &name = ecs.getEntityName(entity);

		j["entities"][name] = json::object();

		auto set = ecs.getEntityComponents(entity);
		for (auto &componentName : set) {
			auto componentJson = ecs.getComponentJson(entity, componentName);
			j["entities"][name][componentName] = componentJson;
		}
	}

	return j;
}

bool InterfaceView::elementExists(const std::string &title) {
	return ecs.getEntityManager().findName(title) < MAX_ENTITIES;
}

EntityID InterfaceView::addElement(const std::string &title) { return ecs.createEntity(title); }

void InterfaceView::removeElement(const std::string &title) {
	EntityID entity = ecs.getEntityManager().findName(title);
	if (entity < MAX_ENTITIES) {
		ecs.destroyEntity(entity);
	}
}

EntityID InterfaceView::getElement(const std::string &title) { return ecs.getEntityManager().findName(title); }

const std::set<EntityID> &InterfaceView::getElements() { return ecs.getEntities(); }

void InterfaceView::renameElement(const std::string &title, const std::string &newTitle) {}

void InterfaceView::changeFocusedElement(const std::string &title) {
	auto entity = ecs.getEntityManager().findName(title);
	if (entity <= MAX_ENTITIES) {
		changeFocusedElement(entity);
	}
}

void InterfaceView::onNotify(Event event) {
	if (current < MAX_ENTITIES) {
		ecs.getSystem().onNotify(event, current);
	}
}

void InterfaceView::update() { ecs.update(); }

void InterfaceView::draw() { ecs.draw(); }

Coordinator &InterfaceView::getCoordinator() { return ecs; }

const std::set<EntityID> &InterfaceView::getEntities() { return ecs.getEntities(); }

void InterfaceView::registerLua(lua_State *L) {
	sol::state_view state{L};

	for (auto &entity : getEntities()) {
		sol::table tbl = state.create_named_table(TextFormat("Entity_%i", entity));

		auto set = ecs.getEntityComponents(entity);
		for (auto &name : set) {
			tbl[name] = ecs.getLuaObject(entity, name, state.lua_state());
		}
	}

	/*
	std::string testCode = R"(
			print('hello, let me test Entity_0.Rectangle')
			print(Entity_0.Rectangle.x)
			print(Entity_0.Rectangle.y)
			print(Entity_0.Rectangle.width)
			print(Entity_0.Rectangle.height)

			print('the entity has a gray color')
			print(Entity_0.ColorRectComponent.Color.r)
			print(Entity_0.ColorRectComponent.Color.g)
			print(Entity_0.ColorRectComponent.Color.b)
			print(Entity_0.ColorRectComponent.Color.a)
		)";
		luaState.script(testCode);
		*/
}

void InterfaceView::changeFocusedElement(EntityID entity) {
	if (entity < MAX_ENTITIES) {
		if (ecs.hasComponent<InputComponent>(entity)) {
			if (current < MAX_ENTITIES) {
				auto &previousInput = ecs.getComponent<InputComponent>(current);
				if (previousInput.callbacks[CALLBACK_UNFOCUSED] != nullptr) {
					previousInput.callbacks[CALLBACK_UNFOCUSED]();
				}
			}

			auto &input = ecs.getComponent<InputComponent>(entity);
			if (input.callbacks[CALLBACK_FOCUSED] != nullptr) {
				input.callbacks[CALLBACK_FOCUSED]();
			}
			current = entity;
		}
	}
}

nlohmann::json InterfaceView::dumpEntityJson(EntityID entity) {
	nlohmann::json j = json::object();

	auto set = ecs.getEntityComponents(entity);
	for (auto &componentName : set) {
		auto componentJson = ecs.getComponentJson(entity, componentName);
		j[componentName] = componentJson;
	}

	return j;
}

void InterfaceView::initEntityComponents(EntityID entity) {
	if (ecs.hasComponent<LabelComponent>(entity)) {
		auto &component = ecs.getComponent<LabelComponent>(entity);
		component.loadFont(component.font.path);
	}

	if (ecs.hasComponent<TextAreaComponent>(entity)) {
		auto &component = ecs.getComponent<TextAreaComponent>(entity);
		component.loadFont(component.font.path);
	}

	if (ecs.hasComponent<ImageRectComponent>(entity)) {
		auto &component = ecs.getComponent<ImageRectComponent>(entity);
		component.scaleImage(component.image.scale);
	}

	if (ecs.hasComponent<NinePatchImageRectComponent>(entity)) {
		auto &component = ecs.getComponent<NinePatchImageRectComponent>(entity);
		component.scaleImage(component.image.scale);
	}

	if (ecs.hasComponent<ButtonComponent>(entity) && ecs.hasComponent<InputComponent>(entity)) {
		auto &input = ecs.getComponent<InputComponent>(entity);
		input.callbacks[CALLBACK_FOCUSED] = [this, entity] {
			auto &button = ecs.getComponent<ButtonComponent>(entity);
			button.shownTextColor = button.focusedTextColor;
			printf("focused..\n");
		};
		input.callbacks[CALLBACK_UNFOCUSED] = [this, entity] {
			auto &button = ecs.getComponent<ButtonComponent>(entity);
			button.shownTextColor = button.normalTextColor;
		};
	}
}
